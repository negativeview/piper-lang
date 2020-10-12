#include "type.hpp"

namespace piper {
    namespace runtime {
        Type::Type(std::string name) {
            this->name = std::move(name);
        }
    }
}