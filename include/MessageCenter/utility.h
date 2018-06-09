#pragma once

#include <memory>


namespace mc {


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


//QVariant parseValue( const QString& value );
//QVariant parseVariant( const QVariant& variant );


}   //  mc::
