#include "../tokenizer/token.hpp"
#include "../logger/logger.hpp"
#include "../runtime/method.hpp"
#include "../runtime/runtime.hpp"
#include "../runtime/struct.hpp"
#include "../runtime/structmember.hpp"
#include "expression.hpp"

#include <vector>

namespace piper {
    namespace parser {
        class Parser {
            public:
                Parser(piper::logger::Logger *logger, piper::runtime::Runtime *runtime);

                void parse(std::vector<piper::tokenizer::Token *> *tokens);
                ~Parser() = default;
            protected:
                piper::runtime::Struct *parseStruct(int *i, std::vector<piper::tokenizer::Token *> *tokens);
                piper::runtime::StructMember *parseStructMember(int *i, std::vector<piper::tokenizer::Token *> *tokens);
                piper::parser::Expression *parsePrimary(int *i, std::vector<piper::tokenizer::Token *> *tokens);
                piper::parser::Expression *parseUnary(int *i, std::vector<piper::tokenizer::Token *> *tokens);
                piper::parser::Expression *parseTerm(int *i, std::vector<piper::tokenizer::Token *> *tokens);
                piper::parser::Expression *parseComparison(int *i, std::vector<piper::tokenizer::Token *> *tokens);
                piper::parser::Expression *parseFactor(int *i, std::vector<piper::tokenizer::Token *> *tokens);
                piper::parser::Expression *parseExpression(int *i, std::vector<piper::tokenizer::Token *> *tokens);
                
                piper::runtime::Method *parseConversionMethod(int *i, std::vector<piper::tokenizer::Token *> *tokens);
                piper::runtime::Method *parseMethod(int *i, std::vector<piper::tokenizer::Token *> *tokens);
                piper::parser::Expression *parseTypeName(int *i, std::vector<piper::tokenizer::Token *> *tokens);

                piper::logger::Logger *logger;
                piper::runtime::Runtime *runtime;
        };
    }
}