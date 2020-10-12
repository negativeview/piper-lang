#include "type.hpp"
#include "variable.hpp"

namespace piper {
    namespace runtime {
        Variable::Variable(Type *type) {
            this->type = type;
            this->ptr = nullptr;
        }

        void Variable::setDataPtr(void *ptr) {
            this->ptr = ptr;
        }

        Type *Variable::getType() {
            return this->type;
        }

        void *Variable::getData() {
            return this->ptr;
        }

        std::string Variable::toString() {
            return this->type->toString(this->ptr);
        }

        bool Variable::isTruthy() {
            return this->type->isTruthy(this->ptr);
        }
    }
}