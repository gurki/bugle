#pragma once

#include "messagecenter/message.h"
#include <string>
#include <memory>


namespace mc {


class Theme;
using ThemePtr = std::shared_ptr<Theme>;
using MessageFormatterRef = std::weak_ptr<class MessageFormatter>;
using MessageFormatterPtr = std::shared_ptr<class MessageFormatter>;


class MessageFormatter
{
    public:

        MessageFormatter();

        virtual std::string format( const Message& message ) const;
        virtual std::string beginColor( const uint8_t index ) const;
        virtual std::string endColor() const;

        std::string space() const { return space_; }
        std::string spacer() const { return spacer_; }
        std::string newline() const { return newline_; }
        std::string skip( const uint8_t count ) const;
        std::string colorize(
            const std::string& text,
            const uint8_t index
        ) const;

        void setIndent( const uint8_t indent );

    protected:

        const std::string spacer_ = u8"\xc2\xb7";   //  &middot
        const std::string space_ = " ";             //  &nbsp;
        const std::string newline_ = "\n";          //  <br>

    private:


//        QString tagInfo( const QVariantMap& tags ) const;
//        QString levelIndent( const Message& message ) const;

        uint8_t indent_ = 0;
        ThemePtr theme_ = nullptr;
};


}   //  mc::