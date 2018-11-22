#pragma once

#include <iostream>
#include <unordered_map>
#include <memory>


namespace mc {


using IndexPair = std::pair<uint8_t, uint8_t>;
using ThemePtr = std::shared_ptr<class Theme>;


class Theme
{
    public:

        Theme();

        void clear();

        void setColor( const std::string& key, const uint8_t primary );
        void setColor( const std::string& key, const uint8_t primary, const uint8_t secondary );

        const IndexPair& indices( const std::string& key ) const;
        const IndexPair& primaryIds() const { return primaryIds_; }
        const IndexPair& secondaryIds() const { return secondaryIds_; }

    private:

        std::unordered_map<std::string, IndexPair> colors_ = {
            { "info", { 39, 45 } },
            { "success", { 40, 46 } },
            { "warning", { 220, 226 } },
            { "error", { 196, 202 } },
            { "debug", { 159, 195 } }
        };
        
        IndexPair primaryIds_ = { 254, 246 };
        IndexPair secondaryIds_ = { 242, 238 };
};


}   //  mc::
