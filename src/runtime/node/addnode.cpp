#include "addnode.hpp"

#include <iostream>

namespace piper {
    namespace runtime {
        namespace node {
            AddNode::AddNode(Node *left, Node *right) {
                this->left = left;
                this->right = right;
            }

            Type *AddNode::getType(Context *context) {
                return nullptr;
            }

            Variable *AddNode::getValue(Context *context) {
                auto *left = dynamic_cast<ValueNode *>(this->left);
                auto *right = dynamic_cast<ValueNode *>(this->right);

                // TODO: Show errors.
                if (left == nullptr) {
                    return nullptr;
                }
                if (right == nullptr) {
                    return nullptr;
                }

                auto a = context->getBinaryFunctionAdd(
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

            std::string AddNode::toString() {
                return "Add";
            }
        }
    }
}