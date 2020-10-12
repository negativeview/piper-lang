#include "i32.hpp"

#include <iostream>
#include <string>

namespace piper {
    namespace runtime {
        namespace types {
            TypeI32::TypeI32() : Type("i32") {
            }

            Variable *TypeI32::getVariableFromString(std::string value) {
                auto variable = new Variable(this);

                // TODO: Allocating every time feels very wasteful
                auto val = static_cast<uint32_t *>(malloc(sizeof(uint32_t)));
                *val = strtol(value.c_str(), nullptr, 0);
                variable->setDataPtr(val);

                return variable;
            }

            std::string TypeI32::toString() {
                return "i32";
            }

            std::string TypeI32::toString(void *data) {
                std::string buffer;

                buffer.append("i32(");
                buffer.append(
                    std::to_string(
                        *static_cast<uint32_t *>(data)
                    )
                );
                buffer += ')';
                return buffer;
            }

            bool TypeI32::isTruthy(void *data) {
                return *static_cast<uint32_t *>(data) != 0;
            }
        }
    }
}