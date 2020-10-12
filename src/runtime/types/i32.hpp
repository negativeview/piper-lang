#pragma once

#include "../type.hpp"
#include "../variable.hpp"

namespace piper {
    namespace runtime {
        namespace types {
            class TypeI32 : public Type {
                public:
                    TypeI32();

                    Variable *getVariableFromString(std::string value);
                    std::string toString(void *);
                    bool isTruthy(void *);
                    std::string toString();

                    ~TypeI32() = default;
                protected:
            };
        }
    }
}