#pragma once

#include "../parser/expression.hpp"

#include <string>

namespace piper {
    namespace runtime {
        class StructMember {
            public:
                StructMember(const std::string &name, const std::string &dataType);
                void addInitializer(piper::parser::Expression *expression);
                ~StructMember();
            protected:
                std::string name;
                std::string dataType;
                piper::parser::Expression *expression;
        };
    }
}