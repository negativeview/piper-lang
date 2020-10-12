#include "variablenode.hpp"

#include <iostream>

namespace piper {
    namespace runtime {
        namespace node {
            VariableNode::VariableNode(Variable *variable) {
                this->variable = variable;
            }

            VariableNode::VariableNode(const std::string &variable_name) {
                this->variable_name = variable_name;
                this->variable = nullptr;
            }

            std::string VariableNode::toString() {
                std::string buffer;
                buffer.append("VariableNode:");
                if (this->variable != nullptr) {
                    buffer.append(this->variable->toString());
                } else {
                    buffer.append(this->variable_name);
                }
                return buffer;
            }

            Variable *VariableNode::getValue(Context *context) {
                if (this->variable != nullptr) {
                    return this->variable;
                }

                return context->getVariableNamed(this->variable_name);
            }

            Type *VariableNode::getType(Context *context) {
                auto a = this->getValue(context);
                if (a == nullptr) {
                    return nullptr;
                }
                return a->getType();
            }
        }
    }
}