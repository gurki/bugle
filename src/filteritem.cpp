#include "messagecenter/filteritem.h"
#include "messagecenter/utility.h"

#include <regex>
#include <string>
#include <iostream>


namespace mc {


////////////////////////////////////////////////////////////////////////////////
FilterItem::FilterItem()
{}


////////////////////////////////////////////////////////////////////////////////
void FilterItem::clear() 
{
    negate_ = false;
    key_.clear();
    value_.clear();
    comparator_ = {};
    plaintext_.clear();
}


////////////////////////////////////////////////////////////////////////////////
bool FilterItem::compare( const nlohmann::json& value ) const {
    return comparator_( value, value_ ) ^ negate_;
}


////////////////////////////////////////////////////////////////////////////////
bool FilterItem::passes( const tags_t& tags ) const
{
    if ( tags.find( key_ ) == tags.end() ) {
        return negate_;
    }

    if ( value_.empty() ) {
        //  implies empty comparator
        return true;
    }

    return compare( tags.at( key_ ) );
}


////////////////////////////////////////////////////////////////////////////////
bool FilterItem::parse( const std::string& plaintext )
{
    static const std::string validOperators = ">=|<=|!=|=|>|<";
    static const std::string validSymbols = "\\w|\\.|-|_|:";
    static const std::string validNames = "((?:" + validSymbols + ")+)";
    static const std::string validValues = "(?:(" + validOperators + ")\\s*" + validNames + ")?";
    static const std::string validRegex = "(!)?\\s*" + validNames + "\\s*" + validValues;
    static std::regex re( validRegex );

    clear();

    std::smatch match;
    std::regex_match( plaintext, match, re );

    if ( match.empty() ) {
        return false;
    }

    //  valid match => non-empty key, optional ( non-empty operator and non-empty value )

    key_ = match[ 2 ];

    const std::string value = match[ 4 ];
    const std::string op = match[ 3 ];

    negate_ = ( match[ 1 ].length() != 0 );
    plaintext_ = ( negate_ ? "!" : "" ) + key_;

    //  early return for key-only
    if ( op.empty() ) {
        return true;
    }

    plaintext_ += op + value;

    try {
        value_ = nlohmann::json::parse( value ); //  number
    } catch ( const std::exception& ) {
        value_ = nlohmann::json::string_t( value );  //  else string
    }

    comparator_ = comparatorForString( op );

    return true;
}


////////////////////////////////////////////////////////////////////////////////
FilterItem::Comparator FilterItem::comparatorForString( const std::string& str )
{
    if ( str == ">=" ) {
        return std::greater_equal<nlohmann::json>();
    } else if ( str == "<=" ) {
        return std::less_equal<nlohmann::json>();
    } else if ( str == "!=" ) {
        return std::not_equal_to<nlohmann::json>();
    } else if ( str == ">" ) {
        return std::greater<nlohmann::json>();
    } else if ( str == "<" ) {
        return std::less<nlohmann::json>();
    } else if ( str == "=" ) {
        return std::equal_to<nlohmann::json>();
    } else {
        return {};
    }
}


////////////////////////////////////////////////////////////////////////////////
bool operator == ( const FilterItem& item1, const FilterItem& item2 ) {
    return item1.plaintext() == item2.plaintext();
}


////////////////////////////////////////////////////////////////////////////////
void to_json( nlohmann::json& j, const FilterItem& item )  {
    j = item.plaintext();
}


////////////////////////////////////////////////////////////////////////////////
void from_json( const nlohmann::json& j, FilterItem& item ) {
    item.parse( j.get<std::string>() );
}


}   //  ::mc