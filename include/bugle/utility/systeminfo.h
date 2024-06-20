#pragma once

#include <string>
#include <chrono>

namespace bugle {


uint64_t get_free_ram();
uint64_t get_total_ram();
std::string get_cpu_model();
std::string get_os_version();
constexpr std::string get_compiler_name();
constexpr std::string get_compiler_version();
constexpr std::string get_os_name();
constexpr std::string get_arch_name();
constexpr std::string get_build_time();
std::string get_local_time();


////////////////////////////////////////////////////////////////////////////////
constexpr std::string get_compiler_version()
{
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


////////////////////////////////////////////////////////////////////////////////
constexpr std::string get_compiler_name()
{
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


////////////////////////////////////////////////////////////////////////////////
constexpr std::string get_os_name()
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
constexpr std::string get_arch_name()
{
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


////////////////////////////////////////////////////////////////////////////////
constexpr std::string get_build_time()
{
    const auto combined = std::format( "{} {}", __DATE__, __TIME__ );
    std::chrono::sys_time<std::chrono::seconds> tp;
    std::istringstream stream( combined );
    stream >> std::chrono::parse( "%b %d %Y %H:%M:%S", tp );
    return std::format( "{:%F}T{:%T}", tp, tp );
}


}   //  ::bugle