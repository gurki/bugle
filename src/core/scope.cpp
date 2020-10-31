#include "bugle/core/scope.h"
#include "bugle/core/messagecenter.h"

namespace bugle {


////////////////////////////////////////////////////////////////////////////////
Scope::Scope( MC_INFO_DECLARE ) :
    Scope( {}, MC_INFO_NAMES )
{}


////////////////////////////////////////////////////////////////////////////////
#ifdef MC_ENABLE
Scope::Scope( const nlohmann::json& tags, MC_INFO_DECLARE ) :
    file_( file ),
    func_( func ),
    line_( line ),
    tags_( bugle::filterTags( tags ) ),
    timestamp_( DateTime::now() ),
    threadId_( std::this_thread::get_id() )
{
    mci.pushScope( *this );
}
#else
Scope::Scope( const nlohmann::json& tags, MC_INFO_DECLARE )
{}
#endif


////////////////////////////////////////////////////////////////////////////////
#ifdef MC_ENABLE
Scope::~Scope() {
    mci.popScope( *this );
}
#else
Scope::~Scope()
{}
#endif


}   //  ::bugle
