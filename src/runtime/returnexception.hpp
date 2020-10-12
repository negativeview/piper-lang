#pragma once

#include "variable.hpp"

namespace piper {
    namespace runtime {
        class ReturnException {
            public:
                ReturnException(Variable *v) {
                    this->variable = v;
                }

                Variable *getVariable() {
                    return this->variable;
                }
            protected:
                Variable *variable;
        };
    }
}