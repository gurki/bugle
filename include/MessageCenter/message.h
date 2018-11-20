#pragma once

#include "messagecenter/datetime.h"

#include <nlohmann/json.hpp>

#include <string>
#include <thread>
#include <chrono>
#include <iostream>


namespace mc {


class jmap_t : public std::unordered_map<std::string, nlohmann::json>
{
    public:

        jmap_t() {}
        jmap_t( const nlohmann::json& jobj ) 
        {
            //  only considers first level strings, objects and convertible arrays
            //  ignores all numbers, bools and second-level non-convertible arrays

            insert( jobj );

            if ( jobj.is_array() ) 
            {
                for ( const auto& tag : jobj ) 
                {
                    if ( tag.is_array() && tag.size() == 2 ) {
                        if ( tag[0].is_string() ) {
                            this->emplace( tag[0], tag[1] );
                        }
                    }
                    else {
                        insert( tag );
                    }
                }
            }
        }

        jmap_t( std::initializer_list<nlohmann::json> init ) :
            jmap_t( nlohmann::json( init ) )
        {}

        void insert( const nlohmann::json& tag ) 
        {
            //  ignores arrays, numbers and bool

            if ( tag.is_string() ) {
                this->emplace( tag, nlohmann::json() );
            }

            if ( tag.is_object() ) {
                for ( auto it = tag.begin(); it != tag.end(); ++it ) {
                    this->emplace( it.key(), it.value() );
                }
            }            
        };

        
        friend std::ostream& operator << ( std::ostream& out, const jmap_t& jmap ) 
        {
            size_t count = 0;

            out << "{";

            for ( const auto& it : jmap ) 
            {
                if ( it.second.is_null() ) {
                    out << it.first;
                }
                else {
                    out << it.first << ":" << it.second;
                }

                if ( count + 1 < jmap.size() ) {
                    out << ", ";
                }

                count++;
            }

            out << "}";

            return out;
        }
};


class Message
{
    public:

        Message();

        Message(
            const char* file,
            const char* function,
            const int line,
            const nlohmann::json& object,
            const jmap_t& tags = {}
        );

//        void set(
//            const std::string& text,
//            const nlohmann::json& tags
//        );
//        void updateTimestamp();

//        const std::chrono::time_point& timestamp() const { return datetime_; }
//        const std::string& text() const { return text_; }
//        const nlohmann::json& tags() const { return tags_; }
//        const std::string& file() const { return file_; }
//        const std::string& function() const { return function_; }
//        int line() const { return line_; }
//        int level() const { return level_; }
//        std::thread::id threadId() const { return threadId_; }

//        std::string info() const;
//        std::string timeInfo( const Qt::DateFormat format = Qt::ISODateWithMs ) const;
        bool hasTags() const { return ! tags_.empty(); }
        bool isIndexed() const { return line_ >= 0; }

//    private:

//        void addSystemTags();

        std::string file_ = {};
        std::string function_ = {};
        int line_ = -1;
        int level_ = -1;

        DateTime datetime_ = {};
        nlohmann::json payload_ = {};
        jmap_t tags_ = {};
        std::thread::id threadId_ = {};
};


}   //  mc::
