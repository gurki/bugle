#pragma once

#include <iostream>
#include <unordered_map>
#include <memory>


namespace bugle {


using ThemePtr = std::shared_ptr<class Theme>;
using DefaultThemePtr = std::shared_ptr<class DefaultTheme>;


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
        void set(
            const std::string& key,
            const std::string& color,
            const std::string& variant
        );

        const ColorPair& primary() const { return primary_; }
        const ColorPair& secondary() const { return secondary_; }
        const ColorPair get( const std::string& key ) const;
        const ColorPair operator[] ( const std::string& key ) const;

    protected:

        std::unordered_map<std::string, ColorPair> colors_ = {};

        ColorPair primary_ = { 15, 15 };
        ColorPair secondary_ = { 15, 15 };
};


class DefaultTheme : public Theme
{
    public:

        DefaultTheme() : Theme()
        {
            colors_ = {
                { "debug", { 123, 195 } },
                { "info", { 39, 81 } },
                { "success", { 40, 83 } },
                { "warning", { 220, 221 } },
                { "error", { 196, 203 } }
            };

            primary_ = { 254, 242 };
            secondary_ = { 248, 236 };
        }
};


}   //  ::bugle
