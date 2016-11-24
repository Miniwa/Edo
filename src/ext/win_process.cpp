#include <windows.h>
#include <tlhelp32.h>
#include <Aclapi.h>

#include "win_process.hpp"
#include "permission.hpp"
#include "process_info.hpp"
#include "error.hpp"


edo::WinProcess::WinProcess()
{
	process_open = false;
	process_handle = NULL;
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

edo::memaddr edo::WinProcess::get_baseaddress()
{
	return get_module_baseaddress(process_info.exe_name);
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

edo::hproc edo::WinProcess::open_secure_process(pid process_id, Permission perm)
{
	PACL pAcl;
	PSECURITY_DESCRIPTOR pSecDesc;
	HANDLE process;
	DWORD result;
	DWORD access_mask = 0;

	switch(perm)
	{
		case Permission::read:
			access_mask = PROCESS_VM_READ;
			break;

		case Permission::write:
			access_mask = PROCESS_VM_WRITE;
			break;

		case Permission::all:
			access_mask = PROCESS_ALL_ACCESS;
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
		throw EdoError("No process opened yet");
}

void edo::WinProcess::throw_if_open()
{
	if (is_open())
		throw EdoError("Process already open");
}