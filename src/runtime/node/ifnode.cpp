#include "ifnode.hpp"

#include "valuenode.hpp"

#include <iostream>

namespace piper {
    namespace runtime {
        namespace node {
            IfNode::IfNode(Node *conditional, ExecutableNode *trueBranch, ExecutableNode *elseBranch) {
                this->conditionalNode = conditional;
                this->trueNode = trueBranch;
                this->falseNode = elseBranch;
            }

            std::string IfNode::toString() {
                return "IfNode";
            }

            Type *IfNode::getType(Context *context) {
                return nullptr;
            }
            
            Variable *IfNode::getValue(Context *context) {
                Variable *out;

                auto *a = dynamic_cast<ValueNode *>(this->conditionalNode);
                if (a == nullptr) {
                    return nullptr;
                }

                if (a->getValue(context)->isTruthy()) {
                    this->trueNode->execute(context, &out);
                } else {
                    this->falseNode->execute(context, &out);
                }

                return out;
            }
        }
    }
}