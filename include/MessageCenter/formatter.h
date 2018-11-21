#pragma once

#include "messagecenter/message.h"
#include <string>
#include <memory>


namespace mc {


using ThemePtr = std::shared_ptr<class Theme>;
using FormatterRef = std::weak_ptr<class Formatter>;
using FormatterPtr = std::shared_ptr<class Formatter>;


class Formatter
{
    public:

        Formatter();

        virtual std::string format( const Message& message ) const;
        virtual std::string beginColor( const uint8_t index ) const { return {}; }
        virtual std::string endColor() const { return {}; }

        std::string space() const { return space_; }
        std::string newline() const { return newline_; }
        std::string skip( const uint8_t count ) const;

        std::string colorize(
            const std::string& text,
            const uint8_t index
        ) const;

        void setIndent( const uint8_t indent );

    protected:

        const std::string space_ = " ";             //  &nbsp;
        const std::string newline_ = "\n";          //  <br>

    private:

        std::string tagInfo( const tags_t& tags ) const;

        uint8_t indent_ = 0;
        ThemePtr theme_ = nullptr;
};


class AsciiFormatter : public Formatter 
{
    public:

        virtual std::string beginColor( const uint8_t index ) const;
        virtual std::string endColor() const;
};


class HtmlFormatter : public Formatter 
{
    public:

        virtual std::string beginColor( const uint8_t index ) const;
        virtual std::string endColor() const;
};


}   //  mc::
