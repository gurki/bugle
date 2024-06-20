#pragma once

#include <string>

namespace bugle {


uint64_t get_free_ram();
uint64_t get_total_ram();
int get_logical_cores();
int get_physical_cores();
std::string get_os_version();
std::string get_kernel_version();
std::string get_compiler_name();
std::string get_compiler_version();
std::string get_os_name();
std::string get_arch_name();
std::string get_cpu_model();
std::string get_local_time();
std::string get_build_time();


}   //  ::bugle