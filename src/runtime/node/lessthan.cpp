#include "lessthan.hpp"

#include "valuenode.hpp"

#include <iostream>

namespace piper {
    namespace runtime {
        namespace node {
            LessThan::LessThan(Runtime *runtime, Node *left, Node *right) {
                this->left = left;
                this->right = right;
                this->runtime = runtime;
            }

            Type *LessThan::getType(Context *context) {
                return this->runtime->getGlobalType("bool");
            }

            std::string LessThan::toString() {
                return "LessThan";
            }

            Variable *LessThan::getValue(Context *context) {
                auto *left = dynamic_cast<ValueNode *>(this->left);
                auto *right = dynamic_cast<ValueNode *>(this->right);

                // TODO: Show errors.
                if (left == nullptr) {
                    return nullptr;
                }
                if (right == nullptr) {
                    return nullptr;
                }

                auto a = context->getComparisonFunctionLessThan(
                    left->getType(context),
                    right->getType(context)
                );
                auto l = left->getValue(context);
                auto r = right->getValue(context);

                return (*a)(context, l, r);
            }
        }
    }
}
