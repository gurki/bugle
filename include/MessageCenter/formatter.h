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
      
        Formatter(
            const std::string& space = u8"\u0020",
            const std::string& spacer = u8"\u00b7\u202f",
            const std::string& newline = "\n"
        );

        virtual std::string format( const Message& message ) const;
        virtual std::string beginColor( const uint8_t index ) const { return {}; }
        virtual std::string endColor() const { return {}; }

        const std::string& space() const { return space_; }
        const std::string& spacer() const { return spacer_; }
        const std::string& newline() const { return newline_; }
        std::string skip( const uint8_t count ) const;
        std::string indent( const int level ) const;

        std::string colorize(
            const std::string& text,
            const uint8_t index
        ) const;

        void setIndent( const uint8_t indent );
        void setTheme( const ThemePtr& theme );

    private:

        const std::string space_;
        const std::string spacer_;
        const std::string newline_;

    private:

        std::string tagInfo( const tags_t& tags ) const;

        uint8_t indent_ = 1;
        ThemePtr theme_ = nullptr;
};


class AsciiFormatter : public Formatter 
{
    public:

        virtual std::string beginColor( const uint8_t index ) const override;
        virtual std::string endColor() const override;
};


class HtmlFormatter : public Formatter 
{
    public:

        HtmlFormatter() : Formatter( "&nbsp;", "<br>", "&middot;" ) {}

        virtual std::string beginColor( const uint8_t index ) const override;
        virtual std::string endColor() const override;
};


}   //  ::mc
