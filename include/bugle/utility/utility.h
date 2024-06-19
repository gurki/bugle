#pragma once

#include <memory>
#include <string>
#include <functional>

namespace bugle {


inline std::string repeat( const std::string& word, size_t times )
{
    std::string result;
    result.reserve( times * word.length() );

    for ( size_t a = 0; a < times; a++ ) {
        result += word;
    }

    return result;
}


template<typename T>
struct WeakPtrHash
{
    size_t operator()( const std::weak_ptr<T>& wp ) const {
        const T* raw = wp.lock().get();
        return std::hash<decltype( raw )>()( raw );
    }
};


template<typename T>
struct WeakPtrEqual {
    bool operator()( const std::weak_ptr<T>& left, const std::weak_ptr<T>& right ) const {
        return ! left.owner_before( right ) && ! right.owner_before( left );
    }
};


}   //  ::bugle
