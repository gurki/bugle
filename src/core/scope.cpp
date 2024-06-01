#include "bugle/core/scope.h"
#include "bugle/core/postoffice.h"

namespace bugle {


////////////////////////////////////////////////////////////////////////////////
Scope::Scope( BUGLE_INFO_DECLARE ) :
    Scope( {}, BUGLE_INFO_NAMES )
{}


////////////////////////////////////////////////////////////////////////////////
#ifdef BUGLE_ENABLE
Scope::Scope( const nlohmann::json& tags, BUGLE_INFO_DECLARE ) :
    file_( file ),
    func_( func ),
    line_( line ),
    tags_( bugle::filterTags( tags ) ),
    timestamp_( DateTime::now() ),
    threadId_( std::this_thread::get_id() )
{
    BUGLE_INSTANCE.pushScope( *this );
}
#else
Scope::Scope( const nlohmann::json& tags, BUGLE_INFO_DECLARE )
{}
#endif


////////////////////////////////////////////////////////////////////////////////
#ifdef BUGLE_ENABLE
Scope::~Scope() {
    BUGLE_INSTANCE.popScope( *this );
}
#else
Scope::~Scope()
{}
#endif


}   //  ::bugle
