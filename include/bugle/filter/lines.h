#pragma once

#include "bugle/core/filter.h"
#include "bugle/core/letter.h"

#include <unordered_map>
#include <vector>
#include <string>

namespace bugle {


struct MessageFilter : public Filter
{
    using comp_t = std::function< bool( const std::string&, const std::string& ) >;

    MessageFilter(
        const std::string& message,
        const comp_t& compare = std::equal_to<>{} )
    {
        matches = [ message, compare ]( const Letter& letter ) {
            return compare( letter.message, message );
        };
    }
};


struct TagFilter : public Filter
{
    TagFilter( const std::string& tag ) {
        matches = [ tag ]( const Letter& letter ) {
            return letter.tags.contains( tag );
        };
    }
};


struct FileFilter : public Filter
{
    FileFilter( const std::string& filename ) {
        matches = [ filename ]( const Letter& letter ) {
            return letter.fileInfo() == filename;
        };
    }
};


struct FunctionFilter : public Filter
{
    FunctionFilter( const std::string& function ) {
        matches = [ function ]( const Letter& letter ) {
            return letter.functionInfo() == function;
        };
    }
};


struct AttributeFilter : public Filter
{
    AttributeFilter( const std::string& key ) {
        matches = [ key ]( const Letter& letter ) {
            return letter.attributes.contains( key );
        };
    }
};


struct ValueFilter : public Filter
{
    using comp_t = std::function< bool( const nlohmann::json&, const nlohmann::json& ) >;

    ValueFilter(
        const std::string& key,
        const nlohmann::json& value,
        const comp_t& compare = std::equal_to<>{} )
    {
        matches = [ key, value, compare ]( const Letter& letter )
        {
            if ( ! letter.attributes.contains( key ) ) {
                return false;
            }

            return compare( letter.attributes.at( key ), value );
        };
    }
};


}   //  ::bugle