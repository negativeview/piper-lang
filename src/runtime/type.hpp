#pragma once

#include "variable.hpp"

#include <string>

namespace piper {
    namespace runtime {
        class Type {
            public:
                Type(std::string name);

                virtual Variable *getVariableFromString(std::string value) = 0;
                virtual std::string toString() = 0;
                virtual std::string toString(void *) = 0;
                virtual bool isTruthy(void *) = 0;

                virtual ~Type() = default;
            protected:
                std::string name;
        };
    }
}