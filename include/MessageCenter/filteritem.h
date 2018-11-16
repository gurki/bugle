#pragma once

#include <nlohmann/json.hpp>

#include <string>
#include <functional>


class FilterItem
{
    public:

        FilterItem();

        bool parse( const std::string& plaintext );
        bool compare( const nlohmann::json& target ) const;

        bool valid() const { return valid_; }
        bool empty() const { return key_.empty(); }
        bool hasValue() const;
        const std::string& key() const { return key_; }
        const nlohmann::json& value() const { return value_; }

    private:

        typedef std::function< bool( const nlohmann::json&, const nlohmann::json& ) > Comparator;

        static Comparator comparatorForString( const std::string& str );

        bool valid_ = false;
        bool negate_ = false;
        std::string key_;
        nlohmann::json value_;
        Comparator comparator_;
};


