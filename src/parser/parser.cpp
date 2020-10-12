#include "parser.hpp"
#include "../runtime/namespace.hpp"
#include "../runtime/struct.hpp"
#include "../runtime/structmember.hpp"
#include "expressions/constant_number_expression.hpp"

#include <iostream>

namespace piper {
    namespace parser {
        Parser::Parser(piper::logger::Logger *logger, piper::runtime::Runtime *runtime) {
            this->logger = logger;
            this->runtime = runtime;
        }

        void Parser::parse(std::vector<piper::tokenizer::Token *> *tokens) {
            int i = 0;
            while (i < tokens->size()) {
                bool isDone = false;

                piper::tokenizer::Token *token = tokens->at(i);

                switch (token->type) {
                    case piper::tokenizer::TokenType::TOKEN_EOF:
                        isDone = true;
                        break;
                    case piper::tokenizer::TokenType::STRUCT:
                        this->parseStruct(&i, tokens);
                        break;
                    case piper::tokenizer::TokenType::IDENTIFIER:
                        {
                            // There are a few things that could happen with an
                            // identifer. Let's try to see which one it is...

                            piper::tokenizer::Token *token2 = tokens->at(i + 1);
                            switch (token2->type) {
                                case piper::tokenizer::TokenType::SKINNY_ARROW:
                                    // Conversion method.
                                    this->parseConversionMethod(&i, tokens);
                                    break;
                                case piper::tokenizer::TokenType::SINGLE_DOT:
                                    {
                                        // Normal member method
                                        piper::runtime::Method *m = this->parseMethod(&i, tokens);
                                        delete m;
                                    }
                                    break;
                                case piper::tokenizer::TokenType::LEFT_PARENTHESIS:
                                    // Normal global method
                                    {
                                        piper::runtime::Method *m = this->parseMethod(&i, tokens);
                                        delete m;
                                    }
                                    break;
                                default:
                                    std::string buffer;
                                    buffer.append("Expected SKINNY_ARROW|SINGLE_DOT after ");
                                    buffer.append(token->str);
                                    buffer.append(". Found ");
                                    buffer.append(token2->str);

                                    this->logger->addError(
                                        token2->start.filename,
                                        token2->start.line,
                                        token2->start.column,
                                        "PARSER",
                                        piper::logger::LoggerSeverity::ERROR,
                                        buffer
                                    );
                                    i++;
                                    break;
                            }
                            break;
                        }
                    default:
                        std::string buffer;
                        buffer.append("Expected STRUCT|EOF|IDENTIFIER. Found ");
                        buffer.append(std::to_string(token->type));

                        this->logger->addError(
                            token->start.filename,
                            token->start.line,
                            token->start.column,
                            "PARSER",
                            piper::logger::LoggerSeverity::ERROR,
                            buffer
                        );
                        i++;
                        break;
                }
                if (isDone) {
                    break;
                }
            }
        }

        piper::parser::Expression *Parser::parseTypeName(int *i, std::vector<piper::tokenizer::Token *> *tokens) {
            piper::tokenizer::Token *a = tokens->at(*i);

            if (a->type != piper::tokenizer::TokenType::IDENTIFIER) {
                // Types always start with an identifier, so this is incorrect.
                return nullptr;
            }

            (*i)++;
            a = tokens->at(*i);

            switch (a->type) {
                case piper::tokenizer::TokenType::LESS_THAN:
                    {
                        (*i)++;
                        this->parseTypeName(i, tokens);
                        a = tokens->at(*i);
                        if (a->type != piper::tokenizer::TokenType::GREATER_THAN) {
                            std::string buffer;
                            buffer.append("Expected > found ");
                            buffer.append(a->str);
                            this->logger->addError(
                                a->start.filename,
                                a->start.line,
                                a->start.column,
                                "PARSER",
                                piper::logger::LoggerSeverity::ERROR,
                                buffer
                            );
                            return nullptr;
                        }
                        // Eat closing >
                        (*i)++;
                    }
                    break;
                default:
                    {
                        std::string buffer;
                        buffer.append("Expected LESS_THAN found ");
                        buffer.append(a->str);
                        this->logger->addError(
                            a->start.filename,
                            a->start.line,
                            a->start.column,
                            "PARSER",
                            piper::logger::LoggerSeverity::ERROR,
                            buffer
                        );
                        return nullptr;
                    }
                    break;
            }

            return nullptr;
        }

        piper::runtime::Method *Parser::parseMethod(int *i, std::vector<piper::tokenizer::Token *> *tokens) {
            piper::tokenizer::Token *a = tokens->at(*i);
            
            if (a->type != piper::tokenizer::TokenType::IDENTIFIER) {
                std::string buffer;
                buffer.append("Expected IDENTIFIER found ");
                buffer.append(a->str);
                this->logger->addError(
                    a->start.filename,
                    a->start.line,
                    a->start.column,
                    "PARSER",
                    piper::logger::LoggerSeverity::ERROR,
                    buffer
                );
                return nullptr;
            }

            std::string id_a = a->str;

            (*i)++;
            a = tokens->at(*i);

            piper::runtime::Method *m;

            if (a->type == piper::tokenizer::TokenType::SINGLE_DOT) {
                // This is a member method.
                const std::string &type_name = id_a;

                (*i)++;
                a = tokens->at(*i);

                if (a->type != piper::tokenizer::TokenType::IDENTIFIER) {
                    std::string buffer;
                    buffer.append("Expected IDENTIFIER found ");
                    buffer.append(a->str);
                    this->logger->addError(
                        a->start.filename,
                        a->start.line,
                        a->start.column,
                        "PARSER",
                        piper::logger::LoggerSeverity::ERROR,
                        buffer
                    );
                    return nullptr;
                }

                std::string method_name = a->str;
                std::cout << "Member method -- type: " << type_name << ", method: " << method_name << std::endl;

                m = new piper::runtime::Method(type_name, method_name);

                (*i)++;
                a = tokens->at(*i);
            } else if (a->type == piper::tokenizer::TokenType::LEFT_PARENTHESIS) {
                // This is a global method.
                const std::string &method_name = id_a;
                std::cout << "Global method -- method: " << method_name << std::endl;

                m = new piper::runtime::Method(method_name);
            } else {
                std::string buffer;
                buffer.append("Expected SINGLE_DOT|LEFT_PARENTHESIS found ");
                buffer.append(a->str);
                this->logger->addError(
                    a->start.filename,
                    a->start.line,
                    a->start.column,
                    "PARSER",
                    piper::logger::LoggerSeverity::ERROR,
                    buffer
                );
                return nullptr;
            }

            if (a->type != piper::tokenizer::TokenType::LEFT_PARENTHESIS) {
                std::string buffer;
                buffer.append("Expected LEFT_PARENTHESIS found ");
                buffer.append(a->str);
                this->logger->addError(
                    a->start.filename,
                    a->start.line,
                    a->start.column,
                    "PARSER",
                    piper::logger::LoggerSeverity::ERROR,
                    buffer
                );
                delete m;

                return nullptr;
            }

            (*i)++;
            a = tokens->at(*i);

            while(true) {
                if (a->type == piper::tokenizer::TokenType::RIGHT_PARENTHESIS) {
                    break;
                }
                
                if (a->type == piper::tokenizer::TokenType::IDENTIFIER) {
                    std::string argName = a->str;

                    (*i)++;
                    a = tokens->at(*i);

                    if (a->type != piper::tokenizer::TokenType::SINGLE_COLON) {
                        std::string buffer;
                        buffer.append("Expected SINGLE_COLON found ");
                        buffer.append(a->str);
                        this->logger->addError(
                            a->start.filename,
                            a->start.line,
                            a->start.column,
                            "PARSER",
                            piper::logger::LoggerSeverity::ERROR,
                            buffer
                        );
                        delete m;
                        return nullptr;
                    }

                    (*i)++;
                    this->parseTypeName(i, tokens);

                    // if (a->type != piper::tokenizer::TokenType::IDENTIFIER) {
                    //     std::string buffer;
                    //     buffer.append("Expected IDENTIFIER found ");
                    //     buffer.append(a->str);
                    //     this->logger->addError(
                    //         a->start.filename,
                    //         a->start.line,
                    //         a->start.column,
                    //         "PARSER",
                    //         piper::logger::LoggerSeverity::ERROR,
                    //         buffer
                    //     );
                    //     return nullptr;
                    // }

                    // std::string typeName = a->str;
                    // (*i)++;
                    a = tokens->at(*i);
                    continue;
                }
                
                std::cout << "Unexpected type: " << a->str << std::endl;
                (*i)++;
                a = tokens->at(*i);
            }

            if (a->type != piper::tokenizer::TokenType::RIGHT_PARENTHESIS) {
                std::string buffer;
                buffer.append("Expected RIGHT_PARENTHESIS found ");
                buffer.append(a->str);
                this->logger->addError(
                    a->start.filename,
                    a->start.line,
                    a->start.column,
                    "PARSER",
                    piper::logger::LoggerSeverity::ERROR,
                    buffer
                );
                delete m;
                return nullptr;
            }

            (*i)++;
            a = tokens->at(*i);

            if (a->type != piper::tokenizer::TokenType::LEFT_CURLY) {
                std::string buffer;
                buffer.append("Expected LEFT_CURLY found ");
                buffer.append(a->str);
                this->logger->addError(
                    a->start.filename,
                    a->start.line,
                    a->start.column,
                    "PARSER",
                    piper::logger::LoggerSeverity::ERROR,
                    buffer
                );
                delete m;
                return nullptr;
            }

            (*i)++;

            while (true) {
                a = tokens->at(*i);
                if (a->type == piper::tokenizer::TokenType::RIGHT_CURLY) {
                    (*i)++;
                    break;
                }

                piper::parser::Expression *expression = this->parseExpression(i, tokens);
                delete expression;
            }

            return m;
        }

        piper::runtime::Method *Parser::parseConversionMethod(int *i, std::vector<piper::tokenizer::Token *> *tokens) {
            piper::tokenizer::Token *a = tokens->at(*i);
            
            if (a->type != piper::tokenizer::TokenType::IDENTIFIER) {
                std::string buffer;
                buffer.append("Expected IDENTIFIER found ");
                buffer.append(a->str);
                this->logger->addError(
                    a->start.filename,
                    a->start.line,
                    a->start.column,
                    "PARSER",
                    piper::logger::LoggerSeverity::ERROR,
                    buffer
                );
                return nullptr;
            }

            std::string typeFrom = a->str;

            (*i)++;
            a = tokens->at(*i);

            if (a->type != piper::tokenizer::TokenType::SKINNY_ARROW) {
                std::string buffer;
                buffer.append("Expected SKINNY_ARROW found ");
                buffer.append(a->str);
                this->logger->addError(
                    a->start.filename,
                    a->start.line,
                    a->start.column,
                    "PARSER",
                    piper::logger::LoggerSeverity::ERROR,
                    buffer
                );
                return nullptr;
            }

            (*i)++;
            a = tokens->at(*i);

            if (a->type != piper::tokenizer::TokenType::IDENTIFIER) {
                std::string buffer;
                buffer.append("Expected IDENTIFIER found ");
                buffer.append(a->str);
                this->logger->addError(
                    a->start.filename,
                    a->start.line,
                    a->start.column,
                    "PARSER",
                    piper::logger::LoggerSeverity::ERROR,
                    buffer
                );
                return nullptr;
            }

            std::string typeTo = a->str;

            (*i)++;
            a = tokens->at(*i);

            if (a->type != piper::tokenizer::TokenType::LEFT_CURLY) {
                std::string buffer;
                buffer.append("Expected LEFT_CURLY found ");
                buffer.append(a->str);
                this->logger->addError(
                    a->start.filename,
                    a->start.line,
                    a->start.column,
                    "PARSER",
                    piper::logger::LoggerSeverity::ERROR,
                    buffer
                );
                return nullptr;
            }

            std::cout << "Conversion method: " << typeFrom << " -> " << typeTo << std::endl;

            (*i)++;

            while (true) {
                a = tokens->at(*i);
                if (a->type == piper::tokenizer::TokenType::RIGHT_CURLY) {
                    (*i)++;
                    return nullptr;
                }

                piper::parser::Expression *expression = this->parseExpression(i, tokens);
                delete expression;
            }

            return nullptr;
        }

        piper::parser::Expression *Parser::parsePrimary(int *i, std::vector<piper::tokenizer::Token *> *tokens) {
            piper::tokenizer::Token *a = tokens->at(*i);

            if (a->type == piper::tokenizer::TokenType::NUMBER) {
                auto expression = new piper::parser::expressions::ConstantNumberExpression(a->str);
                return expression;
            } else if (a->type == piper::tokenizer::TokenType::STRING) {
                std::cout << "Parser::parsePrimary -> STRING (" << a->str << ")" << std::endl;
            } else {
                std::cout << "Parser::parsePrimary -> OTHER (" << std::to_string(a->type) << ": " << a->str << ")" << std::endl;
                return nullptr;
            }

            std::cout << "Primary: " << a->str << std::endl;
            (*i)++;

            return nullptr;
        }

        piper::parser::Expression *Parser::parseUnary(int *i, std::vector<piper::tokenizer::Token *> *tokens) {
            piper::tokenizer::Token *a = tokens->at(*i);

            if (a->type == piper::tokenizer::TokenType::EXCLAMATION || a->type == piper::tokenizer::TokenType::DASH) {
                (*i)++;
                Expression *op = this->parseUnary(i, tokens);
                // We need to parse the a->type and get that somewhere.
                return op;
            }

            return this->parsePrimary(i, tokens);
        }

        piper::parser::Expression *Parser::parseFactor(int *i, std::vector<piper::tokenizer::Token *> *tokens) {
            return this->parseUnary(i, tokens);
        }

        piper::parser::Expression *Parser::parseTerm(int *i, std::vector<piper::tokenizer::Token *> *tokens) {
            piper::parser::Expression *expr = this->parseFactor(i, tokens);
            return expr;
        }

        piper::parser::Expression *Parser::parseComparison(int *i, std::vector<piper::tokenizer::Token *> *tokens) {
            piper::parser::Expression *expr = this->parseTerm(i, tokens);
            (*i)++;
            return expr;
        }

        piper::parser::Expression *Parser::parseExpression(int *i, std::vector<piper::tokenizer::Token *> *tokens) {
            piper::parser::Expression *expression = this->parseComparison(i, tokens);

            piper::tokenizer::Token *a = tokens->at(*i);
            if (a->type == piper::tokenizer::TokenType::SEMICOLON) {
                (*i)++;
                return expression;
            }

            // TODO: Error about lack of semicolon

            return expression;
        }

        piper::runtime::StructMember *Parser::parseStructMember(int *i, std::vector<piper::tokenizer::Token *> *tokens) {
            piper::tokenizer::Token *a = tokens->at(*i);
            std::cout << "Starting to parse struct member: " << a->str << std::endl;

            if (a->type != piper::tokenizer::TokenType::IDENTIFIER) {
                this->logger->addError(a->start.filename, a->start.line, a->start.column, "PARSER", piper::logger::LoggerSeverity::ERROR, "Expected identifier.");
                return nullptr;
            }

            std::string name = a->str;

            (*i)++;
            if (*i >= tokens->size()) {
                this->logger->addError("", 0, 0, "PARSER", piper::logger::LoggerSeverity::ERROR, "Ran out of tokens.");
                return nullptr;
            }
            a = tokens->at(*i);

            if (a->type != piper::tokenizer::TokenType::SINGLE_COLON) {
                this->logger->addError(a->start.filename, a->start.line, a->start.column, "PARSER", piper::logger::LoggerSeverity::ERROR, "Expected `:`.");
                return nullptr;
            }

            (*i)++;
            if (*i >= tokens->size()) {
                this->logger->addError("", 0, 0, "PARSER", piper::logger::LoggerSeverity::ERROR, "Ran out of tokens.");
                return nullptr;
            }
            a = tokens->at(*i);

            if (a->type != piper::tokenizer::TokenType::IDENTIFIER) {
                this->logger->addError(a->start.filename, a->start.line, a->start.column, "PARSER", piper::logger::LoggerSeverity::ERROR, "Expected Type.");
                return nullptr;
            }

            std::string dataType = a->str;

            (*i)++;
            if (*i >= tokens->size()) {
                this->logger->addError("", 0, 0, "PARSER", piper::logger::LoggerSeverity::ERROR, "Ran out of tokens.");
                return nullptr;
            }
            a = tokens->at(*i);

            if (a->type == piper::tokenizer::TokenType::SEMICOLON) {
                (*i)++;
                auto structMember = new piper::runtime::StructMember(name, dataType);
                return structMember;
            } else if (a->type == piper::tokenizer::TokenType::SINGLE_EQUAL) {
                (*i)++;

                auto structMember = new piper::runtime::StructMember(name, dataType);

                piper::parser::Expression *expression = this->parseExpression(i, tokens);
                // TODO: Do we check the expression isn't nullptr here?
                structMember->addInitializer(expression);

                return structMember;
            } else {
                this->logger->addError(a->start.filename, a->start.line, a->start.column, "PARSER", piper::logger::LoggerSeverity::ERROR, "Expected SEMICOLON or SINGLE_EQUAL");
                return nullptr;
            }
            
            return nullptr;
        }

        piper::runtime::Struct *Parser::parseStruct(int *i, std::vector<piper::tokenizer::Token *> *tokens) {
            (*i)++;

            // ERROR
            if (*i >= tokens->size()) {
                this->logger->addError("", 0, 0, "PARSER", piper::logger::LoggerSeverity::ERROR, "Ran out of tokens.");
                return nullptr;
            }

            piper::tokenizer::Token *a = tokens->at(*i);
            if (a->type != piper::tokenizer::TokenType::IDENTIFIER) {
                std::string buffer;
                buffer += "Expected identifier after keyword struct. Found ";
                buffer += a->str;
                buffer += " (";
                buffer += std::to_string(a->type);
                buffer += ")";
                this->logger->addError(a->start.filename, a->start.line, a->start.column, "PARSER", piper::logger::LoggerSeverity::ERROR, buffer);
                return nullptr;
            }

            std::string structName = a->str;

            (*i)++;
            if (*i >= tokens->size()) {
                this->logger->addError("", 0, 0, "PARSER", piper::logger::LoggerSeverity::ERROR, "Ran out of tokens.");
                return nullptr;
            }
            a = tokens->at(*i);

            if (a->type != piper::tokenizer::TokenType::LEFT_CURLY) {
                std::string buffer;
                buffer += "Expected { found ";
                buffer += a->str;
                buffer += " (";
                buffer += std::to_string(a->type);
                buffer += ")";
                this->logger->addError(a->start.filename, a->start.line, a->start.column, "PARSER", piper::logger::LoggerSeverity::ERROR, buffer);
                return nullptr;
            }

            (*i)++;
            if (*i >= tokens->size()) {
                this->logger->addError("", 0, 0, "PARSER", piper::logger::LoggerSeverity::ERROR, "Ran out of tokens.");
                return nullptr;
            }
            // We don't actually store or use a here because that's done in parseStructMember, below.

            piper::runtime::Namespace *ns = this->runtime->addGetNamespace("");
            piper::runtime::Struct *st = ns->createStructShell(structName);

            while (true) {
                piper::runtime::StructMember *member = this->parseStructMember(i, tokens);
                std::cout << "In Struct, new member: " << member << std::endl;
                if (member == nullptr) {
                    // TODO: Is an error.
                    break;
                }

                // TODO: Add member to struct.

                a = tokens->at(*i);
                if (a->type == piper::tokenizer::TokenType::RIGHT_CURLY) {
                    (*i)++;
                    // GOOD BREAK
                    break;
                }
            }

            return st;
        }
    }
}