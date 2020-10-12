#include "returnnode.hpp"

#include "../returnexception.hpp"
#include "valuenode.hpp"

#include <iostream>

namespace piper {
    namespace runtime {
        namespace node {
            ReturnNode::ReturnNode(Node *node) {
                this->node = node;
            }

            Type *ReturnNode::getType(Context *context) {
                return nullptr;
            }

            Variable *ReturnNode::execute(Context *context, Variable **out) {
                auto *node = dynamic_cast<ValueNode *>(this->node);

                auto o = node->getValue(context);
                throw piper::runtime::ReturnException(o);
            }

            Variable *ReturnNode::getValue(Context *context) {
                return nullptr;
            }

            std::string ReturnNode::toString() {
                return "Return";
            }
        }
    }
}