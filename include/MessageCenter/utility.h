#pragma once

#include <memory>
#include <string>


namespace mc {


template<template <typename... Args> class Container, typename Type, typename... Types>
inline bool contains( const Container<Type, Types...>& container, const Type& item ) {
    return container.find( item ) != container.end();
}


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
struct WeakPtrHash : public std::unary_function< std::weak_ptr<T>, size_t >
{
    size_t operator()( const std::weak_ptr<T>& wp ) {
        const T* raw = wp.lock().get();
        return std::hash<decltype( raw )>()( raw );
    }
};


template<typename T>
struct WeakPtrEqual : public std::unary_function< std::weak_ptr<T>, bool >  {
    bool operator()( const std::weak_ptr<T>& left, const std::weak_ptr<T>& right ) {
        return ! left.owner_before( right ) && ! right.owner_before( left );
    }
};


}   //  mc::
