#pragma once

#include "bugle/core/filteritem.h"
#include <nlohmann/json.hpp>
#include <vector>


//  some distant takeaways in the first google result [1]
//  [1] http://booleanblackbelt.com/2008/12/basic-boolean-search-operators-and-query-modifiers-explained/


namespace bugle {


typedef std::vector<FilterItem> FilterConjunction;
typedef std::vector<FilterConjunction> FilterDisjunction;


////////////////////////////////////////////////////////////////////////////////
//  disjunctive normal form of
class BooleanFilter
{
    public:

        BooleanFilter( const std::string& plaintext = {} );

        void clear();
        bool set( const std::string& plaintext );
        bool unite( const std::string& plaintext );
        bool passes( const tags_t& tags ) const;

        size_t size() const { return normalForm_.size(); }
        bool empty() const { return normalForm_.empty(); }
        const std::string& plaintext() const { return plaintext_; }

        friend void to_json( nlohmann::json& j, const BooleanFilter& filter );
        friend void from_json( const nlohmann::json& j, BooleanFilter& filter );

    private:

        FilterDisjunction parseLine( const std::string& plaintext );

        FilterDisjunction normalForm_;    //  or -> and -> (tag, not)
        std::string plaintext_;
};


std::vector<std::string> tokenize(
    const std::string& line,
    const char divider
);


}   //  ::bugle