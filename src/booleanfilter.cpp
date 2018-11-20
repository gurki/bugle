#include "messagecenter/booleanfilter.h"

#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>


namespace mc {


////////////////////////////////////////////////////////////////////////////////
BooleanFilter::BooleanFilter( const std::string& plaintext ) {
    set( plaintext );
}


////////////////////////////////////////////////////////////////////////////////
void BooleanFilter::clear() {
    plaintext_.clear();
    normalForm_.clear();
}


////////////////////////////////////////////////////////////////////////////////
bool BooleanFilter::set( const std::string& plaintext )
{
    clear();

    if ( plaintext.empty() ) {
        return true;
    }

    //  convert linebreaks to conjunctions

    std::string pt = plaintext;
    std::replace( pt.begin(), pt.end(), '\n', ',' );

    if ( pt == plaintext_ ) {
        return true;
    }

    //  parse line
    const FilterDisjunction disjunction = parseLine( pt );

    //  invalid line
    if ( disjunction.empty() ) {
        return false;
    }

    //  set
    
    plaintext_ = plaintext;

    if ( normalForm_ == disjunction ) {
        return true;
    }

    plaintext_ = plaintext;
    normalForm_ = disjunction;

    return true;
}


////////////////////////////////////////////////////////////////////////////////
bool BooleanFilter::unite( const std::string& plaintext )
{
    if ( plaintext.empty() ) {
        return true;
    }

    std::string pt = plaintext;
    std::replace( pt.begin(), pt.end(), '\n', ',' );

    const auto disjunction = parseLine( pt );

    if ( disjunction.empty() ) {
        return false;
    }

    plaintext_ += "," + pt;

    normalForm_.insert( 
        normalForm_.end(),
        disjunction.begin(), disjunction.end()
    );

    return true;
}


////////////////////////////////////////////////////////////////////////////////
FilterDisjunction BooleanFilter::parseLine( const std::string& line )
{
    if ( line.empty() ) {
        return {};
    }

    //  split into disjuncts

    FilterDisjunction disjunction;
    const auto disjuncts = tokenize( line, ',' );

    for ( const auto& disjunct : disjuncts )
    {
        if ( disjunct.empty() ) {
            continue;
        }

        //  split expression into conjuncts

        FilterConjunction conjunction;
        const auto conjuncts = tokenize( disjunct, ' ' );

        for ( const auto& conjunct : conjuncts )
        {
            FilterItem item;
            const bool valid = item.parse( conjunct );

            if ( valid ) {
                conjunction.push_back( item );
            }
        }

        //  add valid option
        if ( ! conjunction.empty() ) {
            disjunction.push_back( conjunction );
        }
    }

    return disjunction;
}


////////////////////////////////////////////////////////////////////////////////
bool BooleanFilter::passes( const jmap_t& tags ) const 
{
    return std::any_of( normalForm_.begin(), normalForm_.end(), [ &tags ]( const FilterConjunction& conjunction ) {
        return std::all_of( conjunction.begin(), conjunction.end(), [ &tags ]( const FilterItem& item ) {
            const bool suc = item.passes( tags );
            return suc;
        });
    });
}


#ifdef NLOHMANN_JSON_HPP

////////////////////////////////////////////////////////////////////////////////
void to_json( nlohmann::json& j, const BooleanFilter& filter ) {
    j = filter.normalForm_;
}


////////////////////////////////////////////////////////////////////////////////
void from_json( const nlohmann::json& j, BooleanFilter& filter ) {
    filter.set( j.get<std::string>() );
}

#endif


////////////////////////////////////////////////////////////////////////////////
std::vector<std::string> tokenize( const std::string& line, const char divider ) 
{
    std::vector<std::string> tokens;
    std::istringstream istr( line );
    std::string token;

    while ( std::getline( istr, token, divider ) ) {
        tokens.push_back( token );
    }

    return tokens;
}


}   //  ::mc