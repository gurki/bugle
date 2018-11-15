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

        const IndexPair& colors( const std::string& key ) const;
        const IndexPair& textColors() const { return textColors_; }

    private:

        std::unordered_map<std::string, IndexPair> colors_;
        IndexPair textColors_ = { 251, 246 };
};


}   //  mc::
