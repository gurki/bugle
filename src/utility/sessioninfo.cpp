#include "bugle/utility/sessioninfo.h"

#include <cstdio>
#include <chrono>
#include <format>
#include <thread>

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


std::string localTime();
std::string systemName();
std::string systemVersion();
std::string systemArchitecture();
std::string cpuModel();
int ramTotalMb();
int ramAvailableMb();


////////////////////////////////////////////////////////////////////////////////
SessionInfo SessionInfo::current() 
{
    SessionInfo info {};
    info.timestamp = bugle::localTime();

#ifdef APP_NAME
    info.appName = APP_NAME;
    info.appVersion = APP_VERSION;
    info.appCommit = APP_COMMIT;
#else
    info.appName = "n/a";
    info.appVersion = "n/a";
    info.appCommit = "n/a";
#endif 

    info.systemName = bugle::systemName();
    info.systemVersion = bugle::systemVersion();
    info.systemArchitecture = bugle::systemArchitecture();
    info.cpuModel = bugle::cpuModel();
    info.cpuCores = std::thread::hardware_concurrency();
    info.ramTotalMb = bugle::ramTotalMb();
    info.ramAvailableMb = bugle::ramAvailableMb();
    return info;
}


////////////////////////////////////////////////////////////////////////////////
int ramAvailableMb()
{
#ifdef _WIN32
    MEMORYSTATUSEX status;
    status.dwLength = sizeof(status);
    if (GlobalMemoryStatusEx(&status)) {
        return status.ullAvailPhys / 1024 / 1024;
    } else {
        return 0; // Failure
    }
#elif defined(__linux__)
    struct sysinfo info;
    if (sysinfo(&info) == 0) {
        return info.freeram * info.mem_unit / 1024 / 1024;
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
    return free_memory / 1024 / 1024;
#else
    return 0; // Unsupported platform
#endif
}


////////////////////////////////////////////////////////////////////////////////
int ramTotalMb()
{
#ifdef _WIN32
    MEMORYSTATUSEX status;
    status.dwLength = sizeof(status);
    if (GlobalMemoryStatusEx(&status)) {
        return status.ullTotalPhys / 1024 / 1024;
    } else {
        return 0; // Failure
    }
#elif defined(__linux__)
    struct sysinfo info;
    if (sysinfo(&info) == 0) {
        return info.totalram * info.mem_unit / 1024 / 1024;
    } else {
        return 0; // Failure
    }
#elif defined(__APPLE__)
    int64_t memsize;
    size_t len = sizeof(memsize);
    if (sysctlbyname("hw.memsize", &memsize, &len, NULL, 0) == 0) {
        return memsize / 1024 / 1024;
    } else {
        return 0; // Failure
    }
#else
    return 0; // Unsupported platform
#endif
}


////////////////////////////////////////////////////////////////////////////////
std::string systemVersion()
{
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


////////////////////////////////////////////////////////////////////////////////
std::string cpuModel()
{
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


////////////////////////////////////////////////////////////////////////////////
std::string localTime()
{
    const auto now = std::chrono::system_clock::now();
    const auto tp = std::chrono::time_point_cast<std::chrono::seconds>( now );
    const auto zt = std::chrono::zoned_time( std::chrono::current_zone(), tp );
    return std::format( "{:%F}T{:%T%z}", zt, zt );
}


////////////////////////////////////////////////////////////////////////////////
std::string systemName()
{
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


////////////////////////////////////////////////////////////////////////////////
std::string systemArchitecture()
{
    std::string arch_name;
#if defined(_M_X64) || defined(__x86_64__)
    arch_name = "AMD64";
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


}   //  ::bugle
