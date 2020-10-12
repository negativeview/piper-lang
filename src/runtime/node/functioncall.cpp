#include "functioncall.hpp"
#include "../returnexception.hpp"

#include <iostream>

namespace piper {
    namespace runtime {
        namespace node {
            FunctionCall::FunctionCall(Method *m) {
                this->method = m;
            }

            std::string FunctionCall::toString() {
                return "FunctionCall";
            }

            Type *FunctionCall::getType(Context *context) {
                return nullptr;
            }

            void FunctionCall::addParameter(
                const std::string &key,
                Node *value
            ) {
                this->parameters[key] = value;
            }

            Variable *FunctionCall::getValue(Context *context) {
                try {
                    return this->method->call(context, this->parameters);
                } catch (ReturnException &v) {
                    std::cout << "Got to return exception" << v.getVariable()->getType() << std::endl;
                    return v.getVariable();
                }
                return nullptr;
            }
        }
    }
}