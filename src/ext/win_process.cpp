#include <windows.h>
#include <tlhelp32.h>
#include <Aclapi.h>

#include "edo/base/error.hpp"
#include "edo/base/strings.hpp"
#include "edo/base/bytebuf.hpp"
#include "edo/ext/win_process.hpp"
#include "edo/ext/permission.hpp"
#include "edo/ext/process_info.hpp"

edo::WinProcess::WinProcess()
{
	construct_default();
}

edo::WinProcess::~WinProcess()
{
	if(is_open())
		close();
}

edo::WinProcess::WinProcess(const WinProcess& right)
{
	WinProcess& right_real = const_cast<WinProcess&>(right);
	copy(right_real);
}

void edo::WinProcess::operator=(const WinProcess right)
{
	WinProcess& right_real = const_cast<WinProcess&>(right);
	copy(right_real);
}

std::vector<edo::ProcessInfo> edo::WinProcess::scan()
{
	HANDLE processSnap;
	PROCESSENTRY32 pe32;
	std::vector<ProcessInfo> result;

	// Try to create a snapshot of all open processes.
	processSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (processSnap == INVALID_HANDLE_VALUE || processSnap == NULL)
		throw EdoError("Could not create process snapshot");

	// Set the size of PE32, not sure why this is necessary but whatever.
	pe32.dwSize = sizeof(PROCESSENTRY32);

	// Try to fetch the first process from the snapshot.
	if (!Process32First(processSnap, &pe32))
	{
		CloseHandle(processSnap);
		throw EdoError("Failed to fetch first process from process snapshot");
	}

	// Start walking the snapshot.
	do
	{
		ProcessInfo pi(
			pe32.szExeFile,
			pe32.th32ProcessID,
			pe32.th32ParentProcessID,
			pe32.cntThreads
		);

		result.push_back(pi);

	} while (Process32Next(processSnap, &pe32));

	// Close the process snapshot handle.
	CloseHandle(processSnap);

	return result;
}

bool edo::WinProcess::is_open()
{
	return process_open;
}

void edo::WinProcess::open(const pid process_id, const Permission perm)
{
	// Check if already open
	throw_if_open();

	// Search for a process with given id
	ProcessInfo pi;
	auto scan_result = WinProcess::scan();
	for (auto it = scan_result.begin(); it != scan_result.end(); it++)
	{
		if (it->process_id == process_id)
		{
			// Copy
			pi = *it;
			break;
		}
	}

	if (pi.process_id == 0)
		throw EdoError("No such process could be found");

	HANDLE proc = open_secure_process(process_id, perm);
	if (proc == INVALID_HANDLE_VALUE || proc == NULL)
		throw EdoError("Could not open process");

	// Get system information.
	SYSTEM_INFO info;
	GetNativeSystemInfo(&info);

	// Get information about the process architecture.
	BOOL isWoW = FALSE;
	BOOL result = IsWow64Process(proc, &isWoW);

	// Check if we could find out whether or not the process is running in Wow64
	if (result == FALSE)
		throw EdoError("Unable to fetch architecture information about process");

	// Check if we're running on Wow64 and if we're running 64bit OS or not.
#ifndef _WIN64
	if (isWoW == FALSE && info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
		throw EdoError("The architecture of the target process is unsupported (64 bit)");
#else
	if (isWoW == TRUE && info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
		throw EdoError("The architecture of the target process is unsupported (32 bit)");
#endif

	process_handle = proc;
	process_info = pi;
	process_open = true;
}

void edo::WinProcess::close()
{
	if (is_open())
	{
		CloseHandle(process_handle);
		process_handle = NULL;
		process_open = false;
	}
}

edo::hproc edo::WinProcess::get_handle()
{
	throw_if_closed();

	HANDLE currentProcess = GetCurrentProcess();
	HANDLE duplicated = NULL;

	BOOL res = DuplicateHandle(currentProcess, process_handle, currentProcess, &duplicated, 0, FALSE, DUPLICATE_SAME_ACCESS);
	if (!res)
		throw edo::EdoError("Could not duplicate handle");

	return duplicated;
}

edo::memaddr edo::WinProcess::get_baseaddress()
{
	return get_module_baseaddress(process_info.get_name());
}

edo::memaddr edo::WinProcess::get_module_baseaddress(const std::string& module)
{
	// Check if closed
	throw_if_closed();

	HANDLE moduleSnapshot = INVALID_HANDLE_VALUE;
	MODULEENTRY32 me32;

	//Try to create a snapshot of loaded modules.
	moduleSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, process_info.process_id);
	if (moduleSnapshot == INVALID_HANDLE_VALUE || moduleSnapshot == NULL)
		throw EdoError("Could not create module snapshot");

	me32.dwSize = sizeof(MODULEENTRY32);

	//Open the first module.
	if (!Module32First(moduleSnapshot, &me32))
	{
		CloseHandle(moduleSnapshot);
		throw EdoError("Could not fetch the first module from module snapshot");
	}

	do
	{
		//Compare the name of the module to try and find a match.
		if (strcmp(me32.szModule, module.c_str()) == 0)
		{
			CloseHandle(moduleSnapshot);
			return reinterpret_cast<memaddr>(me32.modBaseAddr);
		}
	} while (Module32Next(moduleSnapshot, &me32));

	CloseHandle(moduleSnapshot);
	throw EdoError("A module that matches the target name could not be located");
}

bool edo::WinProcess::memread(
	const memaddr address,
	Bytebuf& buffer,
	const std::size_t index,
	const std::size_t byte_count
)
{
	// Check if closed
	throw_if_closed();

	if (index < 0 || index > buffer.size())
		throw std::out_of_range(INDEX_OUT_OF_RANGE);

	if (address == NULL)
		throw std::out_of_range(BAD_PTR);

	// Make sure buffer size can fit the new bytes
	int32_t diff = (index + byte_count) - buffer.size();
	if (diff > 0)
		buffer.pad(diff);

	SIZE_T bytes_read;
	LPCVOID addr = *reinterpret_cast<const LPCVOID*>(&address);
	auto ptr = buffer.data() + index;
	LPVOID dest = *(LPVOID*)&ptr;

	BOOL result = ReadProcessMemory(process_handle, addr, dest, byte_count, &bytes_read);

	return result && bytes_read == byte_count;
}

void edo::WinProcess::safe_memread(
	const memaddr address,
	Bytebuf& buffer,
	const std::size_t index,
	const std::size_t byte_count
)
{
	bool result = memread(address, buffer, index, byte_count);
	if (!result)
		throw EdoError(MEMOP_FAILED);
}

edo::Bytebuf edo::WinProcess::safe_memread(
	const memaddr address,
	const std::size_t byte_count
)
{
	Bytebuf buffer;
	buffer.resize(byte_count);

	bool result = memread(address, buffer, 0, byte_count);
	if (!result)
		throw EdoError(MEMOP_FAILED);

	return buffer;
}

bool edo::WinProcess::memwrite(
	const memaddr address,
	Bytebuf& buffer,
	const std::size_t index,
	const std::size_t byte_count
)
{
	// Check if closed
	throw_if_closed();

	if (index < 0 || index > buffer.size() || (index + byte_count) > buffer.size())
		throw std::out_of_range(INDEX_OUT_OF_RANGE);

	if (address == NULL)
		throw std::out_of_range(BAD_PTR);

	SIZE_T bytes_written;
	LPVOID addr = *(LPVOID*)&address;
	auto ptr = buffer.data() + index;
	LPVOID source = *(LPVOID*)&ptr;

	BOOL result = WriteProcessMemory(process_handle, addr, source, byte_count, &bytes_written);

	return result && bytes_written == byte_count;
}

void edo::WinProcess::safe_memwrite(
	const memaddr address,
	Bytebuf& buffer,
	const std::size_t index,
	const std::size_t byte_count
)
{
	bool result = memwrite(address, buffer, index, byte_count);
	if (!result)
		throw EdoError(MEMOP_FAILED);
}

edo::memaddr edo::WinProcess::follow(
	const memaddr base,
	std::vector<memoffset>::iterator begin,
	std::vector<memoffset>::iterator end
)
{
	// Check if closed
	throw_if_closed();

	memaddr result = base;

	for (auto it = begin; it != end; it++)
	{
		safe_memread<memaddr>(result + *it, &result);
	}

	return result;
}

void edo::WinProcess::construct_default()
{
	process_open = false;
	process_handle = NULL;
	process_info = ProcessInfo();
	internal_buffer = Bytebuf();
}

void edo::WinProcess::copy(WinProcess& right)
{
	// TODO: Fix const correctness on classes in general
	WinProcess& right_real = const_cast<WinProcess&>(right);
	if (right_real.is_open())
	{
		process_open = true;
		process_info = right_real.process_info;
		process_handle = right_real.get_handle();
		internal_buffer = Bytebuf();
	}
	else
		construct_default();
}

edo::hproc edo::WinProcess::open_secure_process(pid process_id, Permission perm)
{
	PACL pAcl;
	PSECURITY_DESCRIPTOR pSecDesc;
	HANDLE process;
	DWORD result;
	DWORD access_mask = 0;

	switch (perm)
	{
		case Permission::read:
			access_mask = PROCESS_VM_READ;
			break;

		case Permission::write:
			access_mask = PROCESS_VM_WRITE;
			break;

		case Permission::all:
			access_mask = PROCESS_ALL_ACCESS;
			break;
	}

	// Try to open the process normally.
	process = OpenProcess(access_mask, false, process_id);
	if (process != INVALID_HANDLE_VALUE && process != NULL)
	{
		return process;
	}

	// A secure process has been encountered.
	// Get the DACL security info of the current process.
	result = GetSecurityInfo(GetCurrentProcess(), SE_KERNEL_OBJECT, DACL_SECURITY_INFORMATION, NULL, NULL, &pAcl, NULL, &pSecDesc);
	if (result != ERROR_SUCCESS)
		return INVALID_HANDLE_VALUE;

	// Try to open the process with write_dac access only.
	process = OpenProcess(WRITE_DAC, NULL, process_id);
	if (process == NULL || process == INVALID_HANDLE_VALUE)
	{
		LocalFree(pSecDesc);
		return INVALID_HANDLE_VALUE;
	}

	// It succeeded.
	// Write the new security info into the target.
	result = SetSecurityInfo(process, SE_KERNEL_OBJECT, DACL_SECURITY_INFORMATION | UNPROTECTED_DACL_SECURITY_INFORMATION, NULL, NULL, pAcl, NULL);
	if (result != ERROR_SUCCESS)
	{
		LocalFree(pSecDesc);
		return INVALID_HANDLE_VALUE;
	}

	// Success. Close some used variables.
	CloseHandle(process);
	LocalFree(pSecDesc);

	//Try to open the process with the original rights.
	process = OpenProcess(access_mask, false, process_id);
	if (process == NULL)
		return INVALID_HANDLE_VALUE;

	//Process is either a valid handle or INVALID_HANDLE_VALUE
	return process;
}

void edo::WinProcess::throw_if_closed()
{
	if(!is_open())
		throw EdoError("No process has been opened yet");
}

void edo::WinProcess::throw_if_open()
{
	if (is_open())
		throw EdoError("A process is already open");
}
