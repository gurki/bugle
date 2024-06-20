#include "bugle/utility/systeminfo.h"

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <format>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#if defined(_WIN32) || defined(__MINGW32__) || defined(__CYGWIN__)
#include <windows.h>
#include <intrin.h>
#elif defined(__linux__) || defined(__APPLE__)
#include <sys/utsname.h>
#include <cstdlib>
#endif

#ifdef __linux__
#include <fstream>
#include <sys/sysinfo.h>
#include <unistd.h>
#elif defined(__APPLE__)
#include <mach/mach.h>
#include <sys/sysctl.h>
#include <sys/types.h>
#endif

namespace bugle {


uint64_t get_free_ram() {
#ifdef _WIN32
    MEMORYSTATUSEX status;
    status.dwLength = sizeof(status);
    if (GlobalMemoryStatusEx(&status)) {
        return status.ullAvailPhys;
    } else {
        return 0; // Failure
    }
#elif defined(__linux__)
    struct sysinfo info;
    if (sysinfo(&info) == 0) {
        return info.freeram * info.mem_unit;
    } else {
        return 0; // Failure
    }
#elif defined(__APPLE__)
    mach_port_t host = mach_host_self();
    vm_size_t page_size;
    vm_statistics64_data_t vm_stats;
    mach_msg_type_number_t count = HOST_VM_INFO64_COUNT;

    if (host_page_size(host, &page_size) != KERN_SUCCESS) {
        return 0; // Failure
    }

    if (host_statistics64(host, HOST_VM_INFO64, (host_info64_t)&vm_stats, &count) != KERN_SUCCESS) {
        return 0; // Failure
    }

    uint64_t free_memory = (vm_stats.free_count + vm_stats.inactive_count) * page_size;
    return free_memory;
#else
    return 0; // Unsupported platform
#endif
}

uint64_t get_total_ram() {
#ifdef _WIN32
    MEMORYSTATUSEX status;
    status.dwLength = sizeof(status);
    if (GlobalMemoryStatusEx(&status)) {
        return status.ullTotalPhys;
    } else {
        return 0; // Failure
    }
#elif defined(__linux__)
    struct sysinfo info;
    if (sysinfo(&info) == 0) {
        return info.totalram * info.mem_unit;
    } else {
        return 0; // Failure
    }
#elif defined(__APPLE__)
    int64_t memsize;
    size_t len = sizeof(memsize);
    if (sysctlbyname("hw.memsize", &memsize, &len, NULL, 0) == 0) {
        return memsize;
    } else {
        return 0; // Failure
    }
#else
    return 0; // Unsupported platform
#endif
}



int get_logical_cores() {
#ifdef _WIN32
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    return sysinfo.dwNumberOfProcessors;
#elif defined(__linux__)
    return sysconf(_SC_NPROCESSORS_ONLN);
#elif defined(__APPLE__)
    int nm[2];
    size_t len = 4;
    uint32_t count;

    nm[0] = CTL_HW;
    nm[1] = HW_AVAILCPU;
    sysctl(nm, 2, &count, &len, NULL, 0);

    if(count < 1) {
        nm[1] = HW_NCPU;
        sysctl(nm, 2, &count, &len, NULL, 0);
        if(count < 1) { count = 1; }
    }
    return count;
#else
    return -1; // Unsupported platform
#endif
}

int get_physical_cores() {
#ifdef _WIN32
    DWORD length = 0;
    GetLogicalProcessorInformation(nullptr, &length);
    std::vector<SYSTEM_LOGICAL_PROCESSOR_INFORMATION> buffer(length / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION));
    GetLogicalProcessorInformation(buffer.data(), &length);

    int physical_cores = 0;
    for (const auto& info : buffer) {
        if (info.Relationship == RelationProcessorCore) {
            physical_cores++;
        }
    }
    return physical_cores;
#elif defined(__linux__)
    std::ifstream cpuinfo("/proc/cpuinfo");
    std::string line;
    int physical_cores = 0;
    while (getline(cpuinfo, line)) {
        if (line.find("cpu cores") != std::string::npos) {
            physical_cores = std::stoi(line.substr(line.find(":") + 2));
            break;
        }
    }
    cpuinfo.close();
    return physical_cores * (get_logical_cores() / std::thread::hardware_concurrency());
#elif defined(__APPLE__)
    int physical_cores;
    size_t len = sizeof(physical_cores);
    sysctlbyname("hw.physicalcpu", &physical_cores, &len, NULL, 0);
    return physical_cores;
#else
    return -1; // Unsupported platform
#endif
}


std::string get_os_version() {
    std::string os_version;

#ifdef _WIN32
    OSVERSIONINFOEX os_info;
    ZeroMemory(&os_info, sizeof(OSVERSIONINFOEX));
    os_info.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

    if (GetVersionEx((OSVERSIONINFO*)&os_info)) {
        os_version = std::to_string(os_info.dwMajorVersion) + "." +
                     std::to_string(os_info.dwMinorVersion) + " (Build " +
                     std::to_string(os_info.dwBuildNumber) + ")";
    } else {
        os_version = "Failed to get Windows version";
    }
#elif defined(__linux__)
    std::ifstream os_release("/etc/os-release");
    std::string line;
    if (os_release.is_open()) {
        while (getline(os_release, line)) {
            if (line.find("PRETTY_NAME=") != std::string::npos) {
                os_version = line.substr(13, line.size() - 14); // Extract value from PRETTY_NAME
                break;
            }
        }
        os_release.close();
    } else {
        struct utsname buffer;
        if (uname(&buffer) == 0) {
            // os_version = buffer.sysname;
            // os_version += " ";
            os_version += buffer.release;
        } else {
            os_version = "Failed to get Linux OS version";
        }
    }
#elif defined(__APPLE__)
    char buffer[256];
    FILE* pipe = popen("sw_vers -productVersion", "r");
    if (pipe) {
        if (fgets(buffer, sizeof(buffer), pipe)) {
            // os_version = "macOS ";
            os_version += buffer;
        }
        pclose(pipe);
    } else {
        struct utsname buffer;
        if (uname(&buffer) == 0) {
            // os_version = buffer.sysname;
            // os_version += " ";
            os_version += buffer.release;
        } else {
            os_version = "Failed to get MacOS version";
        }
    }
#else
    os_version = "Unsupported platform";
#endif

    return os_version;
}

std::string get_kernel_version() {
    std::string kernel_version;

#ifdef _WIN32
    OSVERSIONINFOEX os_info;
    ZeroMemory(&os_info, sizeof(OSVERSIONINFOEX));
    os_info.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

    if (GetVersionEx((OSVERSIONINFO*)&os_info)) {
        kernel_version = "Windows " + std::to_string(os_info.dwMajorVersion) + "." +
                         std::to_string(os_info.dwMinorVersion) + " (Build " +
                         std::to_string(os_info.dwBuildNumber) + ")";
    } else {
        kernel_version = "Failed to get Windows version";
    }
#elif defined(__linux__) || defined(__APPLE__)
    struct utsname buffer;

    if (uname(&buffer) == 0) {
        kernel_version = buffer.sysname;
        kernel_version += " ";
        kernel_version += buffer.release;
    } else {
        kernel_version = "Failed to get Unix-like OS version";
    }
#else
    kernel_version = "Unsupported platform";
#endif

    return kernel_version;
}


struct compiler_info {
    std::string compiler_name;
    std::string compiler_version;
};


compiler_info get_compiler_info() {
    std::string compiler_name;
    std::string compiler_version;
#ifdef _MSC_VER
    compiler_name = "Microsoft Visual Studio";
    compiler_version = std::to_string(_MSC_VER);
#elif defined(__clang__)
    compiler_name = "Clang/LLVM";
    compiler_version = std::to_string(__clang_major__) + "." +
                       std::to_string(__clang_minor__) + "." +
                       std::to_string(__clang_patchlevel__);
#elif defined(__GNUC__) || defined(__GNUG__)
    compiler_name = "GNU Compiler Collection (GCC)";
    compiler_version = std::to_string(__GNUC__) + "." +
                       std::to_string(__GNUC_MINOR__) + "." +
                       std::to_string(__GNUC_PATCHLEVEL__);
#else
    compiler_name = "Unknown Compiler";
    compiler_version = "Unknown Version";
#endif

    return { compiler_name, compiler_version };
}


std::string get_compiler_version() {
#ifdef _MSC_VER
    return std::to_string(_MSC_VER);
#elif defined(__clang__)
    return std::to_string(__clang_major__) + "." +
        std::to_string(__clang_minor__) + "." +
        std::to_string(__clang_patchlevel__);
#elif defined(__GNUC__) || defined(__GNUG__)
    return std::to_string(__GNUC__) + "." +
        std::to_string(__GNUC_MINOR__) + "." +
        std::to_string(__GNUC_PATCHLEVEL__);
#else
    return "Unknown";
#endif
}


std::string get_compiler_name() {
#ifdef _MSC_VER
    return "Microsoft Visual Studio";
#elif defined(__clang__)
    return "Clang/LLVM";
#elif defined(__GNUC__) || defined(__GNUG__)
    return "GCC";
#else
    return "Unknown";
#endif
}


// Function to get the operating system name
std::string get_os_name() {
    std::string os_name;
#ifdef _WIN32
    os_name = "Windows";
#elif defined(__APPLE__) || defined(__MACH__)
    os_name = "macOS";
#elif defined(__linux__)
    os_name = "Linux";
#elif defined(__FreeBSD__)
    os_name = "FreeBSD";
#elif defined(__unix) || defined(__unix__)
    os_name = "Unix";
#else
    os_name = "Unknown OS";
#endif
    return os_name;
}

// Function to get the architecture name
std::string get_arch_name() {
    std::string arch_name;
#if defined(_M_X64) || defined(__x86_64__)
    arch_name = "x64";
#elif defined(_M_IX86) || defined(__i386__)
    arch_name = "x86";
#elif defined(_M_ARM64) || defined(__aarch64__)
    arch_name = "ARM64";
#elif defined(_M_ARM) || defined(__arm__)
    arch_name = "ARM";
#elif defined(__ppc64__) || defined(__powerpc64__)
    arch_name = "PowerPC64";
#elif defined(__ppc__) || defined(__powerpc__)
    arch_name = "PowerPC";
#else
    arch_name = "Unknown Architecture";
#endif
    return arch_name;
}


std::string get_cpu_model() {
    std::string cpu_model;

#ifdef _WIN32
    int cpu_info[4] = { -1 };
    char cpu_brand[0x40];
    __cpuid(cpu_info, 0x80000000);
    unsigned int n_ex_ids = cpu_info[0];
    memset(cpu_brand, 0, sizeof(cpu_brand));

    for (unsigned int i = 0x80000000; i <= n_ex_ids; ++i) {
        __cpuid(cpu_info, i);

        if (i == 0x80000002) {
            memcpy(cpu_brand, cpu_info, sizeof(cpu_info));
        } else if (i == 0x80000003) {
            memcpy(cpu_brand + 16, cpu_info, sizeof(cpu_info));
        } else if (i == 0x80000004) {
            memcpy(cpu_brand + 32, cpu_info, sizeof(cpu_info));
        }
    }
    cpu_model = cpu_brand;

#elif defined(__linux__)
    std::ifstream cpuinfo("/proc/cpuinfo");
    std::string line;
    while (std::getline(cpuinfo, line)) {
        if (line.substr(0, 10) == "model name") {
            cpu_model = line.substr(line.find(":") + 2);
            break;
        }
    }

#elif defined(__APPLE__)
    char buffer[128];
    size_t buffer_size = sizeof(buffer);
    if (sysctlbyname("machdep.cpu.brand_string", &buffer, &buffer_size, NULL, 0) == 0) {
        cpu_model = buffer;
    } else {
        cpu_model = "Unknown CPU Model";
    }

#else
    cpu_model = "Unsupported Platform";
#endif

    return cpu_model;
}


std::string get_local_time() {
    const auto now = std::chrono::system_clock::now();
    const auto tp = std::chrono::time_point_cast<std::chrono::seconds>( now );
    const auto zt = std::chrono::zoned_time( std::chrono::current_zone(), tp );
    return std::format( "{:%F}T{:%T%z}", zt, zt );
}


std::string get_build_time() {
    const auto combined = std::format( "{} {}", __DATE__, __TIME__ );
    std::chrono::sys_time<std::chrono::seconds> tp;
    std::istringstream stream( combined );
    stream >> std::chrono::parse( "%b %d %Y %H:%M:%S", tp );
    return std::format( "{:%F}T{:%T}", tp, tp );
}


}   //  ::bugle