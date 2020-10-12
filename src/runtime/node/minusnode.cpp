#include "minusnode.hpp"

#include <iostream>

namespace piper {
    namespace runtime {
        namespace node {
            MinusNode::MinusNode(Node *left, Node *right) {
                this->left = left;
                this->right = right;
            }

            Type *MinusNode::getType(Context *context) {
                return nullptr;
            }

            Variable *MinusNode::getValue(Context *context) {
                auto *left = dynamic_cast<ValueNode *>(this->left);
                auto *right = dynamic_cast<ValueNode *>(this->right);

                // TODO: Show errors.
                if (left == nullptr) {
                    return nullptr;
                }
                if (right == nullptr) {
                    return nullptr;
                }

                auto a = context->getBinaryFunctionMinus(
                    left->getType(context),
                    right->getType(context)
                );

                auto b = (*a)(
                    context,
                    left->getValue(context),
                    right->getValue(context)
                );

                return b;
            }

            std::string MinusNode::toString() {
                return "Minus";
            }
        }
    }
}