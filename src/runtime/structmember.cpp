#include "structmember.hpp"

namespace piper {
    namespace runtime {
        StructMember::StructMember(const std::string &name, const std::string &dataType) {
            this->name = name;
            this->dataType = dataType;
            this->expression = nullptr;
        }

        void StructMember::addInitializer(piper::parser::Expression *expression) {
            this->expression = expression;
        }
    }
}