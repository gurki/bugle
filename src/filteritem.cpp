#include "messagecenter/filteritem.h"
#include "messagecenter/utility.h"

#include <regex>
#include <string>
#include <iostream>


////////////////////////////////////////////////////////////////////////////////
FilterItem::FilterItem()
{}


////////////////////////////////////////////////////////////////////////////////
bool FilterItem::compare( const nlohmann::json& target ) const {
    return comparator_( target, value_ ) ^ negate_;
}


////////////////////////////////////////////////////////////////////////////////
bool FilterItem::hasValue() const {
    // return ! value_.isNull() && value_.isValid();
    return false;
}


////////////////////////////////////////////////////////////////////////////////
bool FilterItem::parse( const std::string& plaintext )
{
    static const std::string validOperators = "(>=|<=|!=|=|>|<)";
    static const std::string validSymbols = "\\w|\\.|-|_|:";
    static const std::string validNames = "((?:" + validSymbols + ")+)";
    static const std::string validValues = "(?:(?:" + validOperators + validNames + ")?)";

    std::regex re( validNames + validValues );
    std::cmatch m;
    std::regex_match( plaintext, m, re );

    std::cout << m.size() << std::endl;
    // static const QRegularExpression re( validNames + validValues );

    // const QRegularExpressionMatch match = re.match( plaintext );

    // if ( ! match.hasMatch() ) {
    //     valid_ = false;
    //     return false;
    // }

    // //  valid match => non-empty key, optional ( non-empty operator and non-empty value )

    // valid_ = true;
    // key_ = match.captured( 1 );

    // const auto valueStr = match.captured( 3 );
    // const auto op = match.captured( 2 );

    // //  early return for key-only
    // if ( op.isEmpty() ) {
    //     return true;
    // }

    // value_ = mc::parseValue( valueStr );
    // comparator_ = comparatorForString( op );

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
        assert( "bug in comparator parsing" );
        return Comparator();
    }
}
