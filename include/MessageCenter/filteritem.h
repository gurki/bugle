#pragma once

#include "messagecenter/message.h"

#include <nlohmann/json.hpp>

#include <string>
#include <functional>


namespace mc {


class FilterItem
{
    public:

        FilterItem();

        void clear();
        bool parse( const std::string& plaintext );
        bool compare( const nlohmann::json& value ) const;
        bool passes( const jmap_t& set ) const;

        bool empty() const { return key_.empty(); }
        bool hasValue() const { return ! empty() && ! value_.empty(); };
        const std::string& key() const { return key_; }
        const nlohmann::json& value() const { return value_; }
        const std::string& plaintext() const { return plaintext_; }

    private:

        typedef std::function< bool( const nlohmann::json&, const nlohmann::json& ) > Comparator;

        static Comparator comparatorForString( const std::string& str );

        bool negate_ = false;
        std::string key_ = {};
        nlohmann::json value_ = {}; //  number or string only
        Comparator comparator_ = {};
        std::string plaintext_ = {};
};


bool operator == ( const FilterItem& item1, const FilterItem& item2 );


#ifdef NLOHMANN_JSON_HPP
    void to_json( nlohmann::json& j, const FilterItem& item );
    void from_json( const nlohmann::json& j, FilterItem& item );
#endif


}   //  ::mc