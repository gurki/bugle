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

    std::string message;
    comp_t compare = std::equal_to<>{};

    MessageFilter(
        const std::string& _message,
        const comp_t& _compare = std::equal_to<>{} ) :
        message( _message ),
        compare( _compare )
    {
        matches = [ this ]( const Letter& letter ) {
            return compare( letter.message, message );
        };
    }
};


struct TagFilter : public Filter
{
    std::string tag;

    TagFilter( const std::string& _tag ) :
        tag( _tag )
    {
        matches = [ this ]( const Letter& letter ) {
            return letter.tags.contains( tag );
        };
    }
};


struct FileFilter : public Filter
{
    std::string filename;

    FileFilter( const std::string& _filename ) :
        filename( _filename )
    {
        matches = [ this ]( const Letter& letter ) {
            return letter.fileInfo() == filename;
        };
    }
};


struct FunctionFilter : public Filter
{
    std::string function;

    FunctionFilter( const std::string& _function ) :
        function( _function )
    {
        matches = [ this ]( const Letter& letter ) {
            return letter.functionInfo() == function;
        };
    }
};


struct AttributeFilter : public Filter
{
    std::string key;

    AttributeFilter( const std::string& _key ) :
        key( _key )
    {
        matches = [ this ]( const Letter& letter ) {
            return letter.attributes.contains( key );
        };
    }
};


struct ValueFilter : public Filter
{
    using comp_t = std::function< bool( const nlohmann::json&, const nlohmann::json& ) >;
    std::string key;
    nlohmann::json value;
    comp_t compare = std::equal_to<>{};

    ValueFilter(
        const std::string& _key,
        const nlohmann::json& _value,
        const comp_t& _compare = std::equal_to<>{} ) :
        key( _key ),
        value( _value ),
        compare( _compare )
    {
        matches = [ this ]( const Letter& letter ) {
            if ( ! letter.attributes.contains( key ) ) {
                return false;
            }

            return compare( letter.attributes.at( key ), value );
        };
    }

    static FilterPtr fromString( const std::string& );
};


}   //  ::bugle