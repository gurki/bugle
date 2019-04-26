#include "messagecenter/scope.h"
#include "messagecenter/messagecenter.h"

namespace mc {


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
    tags_( mc::filterTags( tags ) ),
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


}   //  ::mc
