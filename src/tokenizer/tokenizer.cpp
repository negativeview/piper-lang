#include "tokenizer.hpp"
#include "location.hpp"

#include <fstream>
#include <iostream>
#include <map>

namespace piper {
    namespace tokenizer {
        Tokenizer::Tokenizer(piper::logger::Logger *logger) {
            this->logger = logger;

            keywords["struct"] = { TokenType::STRUCT };

            simpleTokens['{'] = { TokenType::LEFT_CURLY        };
            simpleTokens['}'] = { TokenType::RIGHT_CURLY       };
            simpleTokens['['] = { TokenType::LEFT_SQUARE       };
            simpleTokens[']'] = { TokenType::RIGHT_SQUARE      };
            simpleTokens['('] = { TokenType::LEFT_PARENTHESIS  };
            simpleTokens[')'] = { TokenType::RIGHT_PARENTHESIS };
            simpleTokens[';'] = { TokenType::SEMICOLON         };
            simpleTokens[','] = { TokenType::COMMA             };
            simpleTokens['%'] = { TokenType::PERCENT           };
            simpleTokens['~'] = { TokenType::TILDE             };
            simpleTokens['|'] = { TokenType::PIPE              };
            simpleTokens[':'] = { TokenType::SINGLE_COLON      };
            simpleTokens['.'] = { TokenType::SINGLE_DOT        };

            auto dashExtensions = new std::map<char, SimpleTokenConfig>();
            (*dashExtensions)['>'] = {
                TokenType::SKINNY_ARROW
            };
            (*dashExtensions)['='] = {
                TokenType::DASH_EQUALS
            };
            (*dashExtensions)['-'] = {
                TokenType::DASH_DASH
            };

            doubleTokens['-'] = {
                {
                    TokenType::DASH
                },
                dashExtensions
            };

            auto questionExtensions = new std::map<char, SimpleTokenConfig>();
            (*questionExtensions)[':'] = {
                TokenType::QUESTION_COLON
            };

            doubleTokens['?'] = {
                {
                    TokenType::QUESTION
                },
                questionExtensions
            };

            auto lessThanExtensions = new std::map<char, SimpleTokenConfig>();
            (*lessThanExtensions)['='] = {
                TokenType::LESS_THAN_EQUAL
            };

            doubleTokens['<'] = {
                {
                    TokenType::LESS_THAN
                },
                lessThanExtensions
            };

            auto greaterThanExtensions = new std::map<char, SimpleTokenConfig>();
            (*greaterThanExtensions)['='] = {
                TokenType::GREATER_THAN_EQUAL
            };

            doubleTokens['>'] = {
                {
                    TokenType::GREATER_THAN
                },
                greaterThanExtensions
            };

            auto asteriskExtensions = new std::map<char, SimpleTokenConfig>();
            (*asteriskExtensions)['*'] = {
                TokenType::ASTERISK_ASTERISK
            };
            (*asteriskExtensions)['='] = {
                TokenType::ASTERISK_EQUALS
            };

            doubleTokens['*'] = {
                {
                    TokenType::ASTERISK
                },
                asteriskExtensions
            };

            auto exclamationExtensions = new std::map<char, SimpleTokenConfig>();
            (*exclamationExtensions)['='] = {
                TokenType::EXCLAMATION_EQUAL
            };

            doubleTokens['!'] = {
                {
                    TokenType::EXCLAMATION
                },
                exclamationExtensions
            };

            auto forwardSlashExtensions = new std::map<char, SimpleTokenConfig>();
            (*forwardSlashExtensions)['='] = {
                TokenType::FORWARD_SLASH_EQUALS
            };

            doubleTokens['/'] = {
                {
                    TokenType::FORWARD_SLASH
                },
                forwardSlashExtensions
            };

            auto equalExtensions = new std::map<char, SimpleTokenConfig>();
            (*equalExtensions)['>'] = {
                TokenType::FAT_ARROW
            };
            (*equalExtensions)['='] = {
                TokenType::DOUBLE_EQUAL
            };

            doubleTokens['='] = {
                {
                    TokenType::SINGLE_EQUAL
                },
                equalExtensions
            };

            auto plusExtensions = new std::map<char, SimpleTokenConfig>();
            (*plusExtensions)['+'] = {
                TokenType::PLUS_PLUS
            };
            (*plusExtensions)['='] = {
                TokenType::PLUS_EQUALS
            };

            doubleTokens['+'] = {
                {
                    TokenType::SINGLE_PLUS
                },
                plusExtensions
            };
        }

        std::vector<Token *> *Tokenizer::tokenizeAsync(
            const std::string &filename
        ) {
            auto tokens = new std::vector<Token *>();

            std::ifstream file;
            file.open(filename);

            int line = 1;
            int column = 1;

            while (true) {
                if (file.eof()) {
                    break;
                }

                Location start_location;
                start_location.filename = filename;

                Location end_location;
                end_location.filename = filename;

                char c = file.peek();
                if (c == EOF) {
                    break;
                }

                if (c == '\n') {
                    line++;
                    column = 1;
                    file.get();
                    continue;
                }

                if (isspace(c) != 0) {
                    column++;
                    file.get();
                    continue;
                }

                if (simpleTokens.find(c) != simpleTokens.end()) {
                    start_location.line = line;
                    start_location.column = column;

                    column++;
                    file.get();

                    end_location.line = line;
                    end_location.column = column;
                    
                    auto t = new Token();
                    t->start = start_location;
                    t->end = end_location;
                    t->str = c;
                    t->type = simpleTokens.find(c)->second.type;
                    tokens->push_back(t);

                    continue;
                }

                if (doubleTokens.find(c) != doubleTokens.end()) {
                    start_location.line = line;
                    start_location.column = column;

                    std::string buffer;
                    buffer += c;

                    column++;
                    file.get();

                    auto def = doubleTokens.find(c)->second.first;
                    auto possibilities = doubleTokens.find(c)->second.second;

                    c = file.peek();

                    if (possibilities->find(c) != possibilities->end()) {
                        column++;
                        file.get();

                        end_location.line = line;
                        end_location.column = column;

                        buffer += c;

                        auto t = new Token();
                        t->start = start_location;
                        t->end = end_location;
                        t->str += buffer;
                        t->type = possibilities->find(c)->second.type;
                        tokens->push_back(t);
                        continue;
                    } else {
                        end_location.line = line;
                        end_location.column = column;

                        auto t = new Token();
                        t->start = start_location;
                        t->end = end_location;
                        t->str += buffer;
                        t->type = def.type;
                        tokens->push_back(t);
                        continue;
                    }
                }
                
                if (c == '`') {
                    start_location.line = line;
                    start_location.column = column;

                    column++;
                    file.get();

                    c = file.peek();

                    std::string buffer;
                    while (true) {
                        if (c == '`') {
                            file.get();
                            column++;
                            break;
                        }

                        c = file.peek();
                        if (c == '\\') {
                            file.get();
                            column++;

                            c = file.get();
                            buffer += c;
                            c = file.peek();
                            column++;
                            continue;
                        } else {
                            c = file.get();
                            buffer += c;
                            column++;
                            c = file.peek();
                        }
                    }

                    end_location.line = line;
                    end_location.column = column;
                    
                    auto t = new Token();
                    t->start = start_location;
                    t->end = end_location;
                    t->str = buffer;
                    t->type = TokenType::FORMAT_STRING;
                    tokens->push_back(t);

                    continue;
                }

                if (c == '"') {
                    start_location.line = line;
                    start_location.column = column;

                    column++;
                    file.get();

                    c = file.peek();

                    std::string buffer;
                    while (true) {
                        if (c == '"') {
                            file.get();
                            column++;
                            break;
                        }

                        c = file.peek();
                        if (c == '\\') {
                            file.get();
                            column++;

                            c = file.get();
                            buffer += c;
                            c = file.peek();
                            column++;
                            continue;
                        } else {
                            c = file.get();
                            buffer += c;
                            column++;
                            c = file.peek();
                        }
                    }

                    end_location.line = line;
                    end_location.column = column;
                    
                    auto t = new Token();
                    t->start = start_location;
                    t->end = end_location;
                    t->str = buffer;
                    t->type = TokenType::STRING;
                    tokens->push_back(t);

                    continue;
                }

                if (isdigit(c) != 0) {
                    std::string buffer;

                    start_location.line = line;
                    start_location.column = column;

                    while (true) {
                        buffer += c;

                        file.get();
                        column++;

                        c = file.peek();
                        if (isdigit(c) == 0) {
                            end_location.line = line;
                            end_location.column = column;
                            break;
                        }
                    }

                    auto t = new Token();
                    t->start = start_location;
                    t->end = end_location;
                    t->str = buffer;
                    t->type = TokenType::NUMBER;
                    tokens->push_back(t);

                    continue;
                }

                if (isalpha(c) != 0 || c == '_') {
                    std::string buffer;

                    start_location.line = line;
                    start_location.column = column;

                    while (true) {
                        buffer += c;

                        file.get();
                        column++;

                        c = file.peek();
                        if (!(isalnum(c) != 0 || c == '_')) {
                            end_location.line = line;
                            end_location.column = column;
                            break;
                        }
                    }

                    auto t = new Token();
                    t->start = start_location;
                    t->end = end_location;
                    t->str = buffer;

                    if (keywords.find(buffer) != keywords.end()) {
                        t->type = keywords.find(buffer)->second.type;
                    } else {
                        t->type = TokenType::IDENTIFIER;
                    }
                    tokens->push_back(t);

                    continue;
                }

                std::string buffer;
                buffer.append("Unknown character: ");
                buffer += c;
                this->logger->addError(filename, line, column, "TOKENIZER", piper::logger::LoggerSeverity::ERROR, buffer);

                column++;
                file.get();
            }

            file.close();

            auto t = new Token();
            t->type = TokenType::TOKEN_EOF;
            tokens->push_back(t);

            return tokens;
        }
    }
}