#pragma once

#include <iostream>
#include <unordered_map>
#include <memory>


namespace mc {


using ThemePtr = std::shared_ptr<class Theme>;


struct ColorPair {
    uint8_t color;
    uint8_t variant;
};


class Theme
{
    public:

        Theme() {}

        void clear();

        void setPrimary( const uint8_t primaryId, const uint8_t variantId );
        void setSecondary( const uint8_t secondaryId, const uint8_t variantId );
        void set( const std::string& key, const uint8_t colorId );
        void set( 
            const std::string& key, 
            const uint8_t colorId, 
            const uint8_t variantId 
        );

        const ColorPair& primary() const { return primary_; }
        const ColorPair& secondary() const { return secondary_; }
        const ColorPair get( const std::string& key ) const;
        const ColorPair operator[] ( const std::string& key ) const;

    private:

        std::unordered_map<std::string, ColorPair> colors_ = {
            { "info", { 39, 45 } },
            { "success", { 40, 46 } },
            { "warning", { 220, 226 } },
            { "error", { 196, 202 } },
            { "debug", { 159, 195 } }
        };
        
        ColorPair primary_ = { 254, 246 };
        ColorPair secondary_ = { 242, 238 };
};




}   //  mc::
