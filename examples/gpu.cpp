//  gpu demo: adapter capabilities queried through webgpu, rendered via
//  the generic banner convention. requires -DBUGLE_MODULE_GPU=ON.

#include <bugle/bugle.h>


////////////////////////////////////////////////////////////////////////////////
int main()
{
    auto& po = bugle::PostOffice::instance();

    auto console = std::make_shared<bugle::ConsoleLogger>();
    po.addObserver( console );

    //  first call requests the adapter; results are cached
    const nlohmann::json gpuInfo = bugle::GpuInfo::current();
    po.post( {}, { "gpu", "system" }, gpuInfo );

    po.flush();
}
