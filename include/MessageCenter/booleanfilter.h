#pragma once

#include "messagecenter/filteritem.h"
#include <nlohmann/json.hpp>
#include <vector>


//  some distant takeaways in the first google result [1]
//  [1] http://booleanblackbelt.com/2008/12/basic-boolean-search-operators-and-query-modifiers-explained/


typedef std::vector<FilterItem> FilterConjunction;
typedef std::vector<FilterConjunction> FilterDisjunction;


////////////////////////////////////////////////////////////////////////////////
//  disjunctive normal form of
class BooleanFilter
{
    public:

        BooleanFilter( const std::string& plaintext = {} );

        bool set( const std::string& plaintext );
        bool unite( const std::string& line );
        bool passes( const nlohmann::json& set ) const;

        size_t size() const { return normalForm_.size(); }
        bool empty() const { return normalForm_.empty(); }
        const std::string& plaintext() const { return plaintext_; }

        static bool passes(
            const FilterItem& item,
            const nlohmann::json& set
        );

        static bool isSubset(
            const FilterConjunction& conjunct,
            const nlohmann::json& set
        );

        static bool containsSubset(
            const FilterDisjunction& disjunct,
            const nlohmann::json& set
        );

    private:

        FilterDisjunction parseLine( const std::string& plaintext );

        FilterDisjunction normalForm_;    //  or -> and -> (tag, not)
        std::string plaintext_;
};
