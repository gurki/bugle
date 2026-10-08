#include <bugle/bugle.h>
int main() {
    bugle::PostOffice office;
#ifdef HAVE_CONSOLE
    auto console = std::make_shared<bugle::ConsoleLogger>();
    office.addObserver( console );
#endif
#ifdef HAVE_JSON
    bugle::JsonLogger json;
#endif
#ifdef HAVE_PROFILER
    bugle::Profiler profiler;
#endif
#ifdef HAVE_SYSINFO
    auto info = bugle::BuildInfo::current();
    auto session = bugle::SessionInfo::current();
#endif
    office.post( "installed consumer" );
    office.flush();
}
