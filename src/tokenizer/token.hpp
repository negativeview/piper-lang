#pragma once

#include "location.hpp"

namespace piper {
    namespace tokenizer {
        enum TokenType {
            ASTERISK,
            ASTERISK_ASTERISK,
            ASTERISK_EQUALS,
            IDENTIFIER,
            LEFT_CURLY,
            RIGHT_CURLY,
            SKINNY_ARROW,
            FAT_ARROW,
            LEFT_PARENTHESIS,
            RIGHT_PARENTHESIS,
            DASH,
            DASH_DASH,
            DASH_EQUALS,
            SINGLE_COLON,
            SINGLE_DOT,
            SEMICOLON,
            LEFT_SQUARE,
            RIGHT_SQUARE,
            SINGLE_EQUAL,
            DOUBLE_EQUAL,
            STRING,
            FORMAT_STRING,
            NUMBER,
            COMMA,
            SINGLE_PLUS,
            PLUS_PLUS,
            PLUS_EQUALS,
            FORWARD_SLASH,
            FORWARD_SLASH_EQUALS,
            EXCLAMATION,
            EXCLAMATION_EQUAL,
            TILDE,
            PERCENT,
            LESS_THAN,
            LESS_THAN_EQUAL,
            GREATER_THAN,
            GREATER_THAN_EQUAL,
            PIPE,
            QUESTION,
            QUESTION_COLON,
            TOKEN_EOF,

            STRUCT
        };

        struct Token {
            TokenType type;
            std::string str;
            Location start;
            Location end;
        };
    }
}