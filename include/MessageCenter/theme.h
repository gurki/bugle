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

        Theme() {}

        void clear();

        void setPrimary( const uint8_t primaryId, const uint8_t variantId );
        void setSecondary( const uint8_t secondaryId, const uint8_t variantId );
        void setColor( const std::string& key, const uint8_t colorId );
        void setColor( 
            const std::string& key, 
            const uint8_t colorId, 
            const uint8_t variantId 
        );

        const IndexPair& primary() const { return primary_; }
        const IndexPair& secondary() const { return secondary_; }
        const IndexPair indices( const std::string& key ) const;
        const IndexPair operator[] ( const std::string& key ) const;

    private:

        std::unordered_map<std::string, IndexPair> colors_ = {
            { "info", { 39, 45 } },
            { "success", { 40, 46 } },
            { "warning", { 220, 226 } },
            { "error", { 196, 202 } },
            { "debug", { 159, 195 } }
        };
        
        IndexPair primary_ = { 254, 246 };
        IndexPair secondary_ = { 242, 238 };
};




}   //  mc::
