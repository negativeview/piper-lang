#pragma once

#include "../expression.hpp"

namespace piper {
    namespace parser {
        namespace expressions {
            class ConstantNumberExpression : public Expression {
                public:
                    ConstantNumberExpression(std::string value) {
                        this->str_value = value;
                    }

                    std::string toString() {
                        return this->str_value;
                    }
                protected:
                    std::string str_value;
            };
        }
    }
}