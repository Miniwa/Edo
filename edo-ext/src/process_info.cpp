#include "process_info.hpp"

edo::ProcessInfo::ProcessInfo(
    const std::string& name,
    const pid process_id,
    const pid parent,
    const uint32_t threads
)
{
    this->exe_name = name;
    this->process_id = process_id;
    this->parent_process_id = parent;
    this->thread_count = threads;
}