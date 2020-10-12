#pragma once

#include "token.hpp"
#include "../logger/logger.hpp"

#include <map>
#include <string>
#include <functional>
#include <vector>

namespace piper {
    namespace tokenizer {
        struct SimpleTokenConfig {
            TokenType type;
        };

        class Tokenizer {
            public:
                Tokenizer(piper::logger::Logger *logger);

                std::vector<Token *> *tokenizeAsync(
                    const std::string &filename
                );
            protected:
                piper::logger::Logger *logger;

                std::map<std::string, SimpleTokenConfig> keywords;
                std::map<char, SimpleTokenConfig> simpleTokens;
                std::map<
                    char, // start of sequence
                    std::pair<
                        SimpleTokenConfig, // token if none of the following
                        std::map<
                            char, // possible second character in sequence
                            SimpleTokenConfig // token if second
                        > *
                    >
                > doubleTokens;

        };
    }
}