#include <bugle/utility.h>

#include <chrono>
#include <iostream>
#include <print>
#include <thread>

using namespace std;
using namespace bugle;


int main()
{
    nlohmann::json build = BuildInfo::current();
    nlohmann::json session = SessionInfo::current();
    nlohmann::json gpu = GpuInfo::current();

    std::println( "{}", build.dump( 2 ) );
    std::println( "{}", session.dump( 2 ) );
    std::println( "{}", gpu.dump( 2 ) );
    
    // cout << endl << "/// SESSION ///" << endl;
    // cout << "time:              " << get_local_time() << endl;

    // cout << endl << "/// APP ///" << endl;
    // cout << "name:              " << APP_NAME << endl;
    // cout << "version:           " << APP_VERSION << endl;
    // cout << "commit:            " << APP_COMMIT << endl;

    // cout << endl << "/// OS ///" << endl;
    // cout << "version:           " << std::format( "{} {}", get_os_name(), get_os_version() );
    // // cout << "version:           " << get_os_version();
    // cout << "architecture:      " << get_arch_name() << endl;
    // // cout << "kernel:            " << get_kernel_version() << endl;

    // cout << endl << "/// CPU ///" << endl;
    // cout << "model:             " << get_cpu_model() << endl;
    // cout << "cores:             " << std::thread::hardware_concurrency() << endl;

    // cout << endl << "/// RAM ///" << endl;
    // cout << "total:             " << std::format( "{:.2f} GiB", get_total_ram() / 1024 / 1024 / 1024.f ) << endl;
    // cout << "free:              " << std::format( "{:.2f} GiB", get_free_ram() / 1024 / 1024 / 1024.f ) << endl;

    return 0;
}
