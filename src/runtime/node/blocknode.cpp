#include "blocknode.hpp"

#include "valuenode.hpp"

#include <iostream>

namespace piper {
    namespace runtime {
        namespace node {
            std::string BlockNode::toString() {
                return "BlockNode";
            }

            Variable *BlockNode::execute(Context *context, Variable **out) {
                auto c2 = new Context(context, true);

                for (auto a: this->nodes) {
                    ExecutableNode *en = dynamic_cast<ExecutableNode *>(a);
                    if (en != nullptr) {
                        en->execute(context, out);
                        if (*out != nullptr) {
                            std::cout << "Block::execute::1::" << *out << ":" << (*out)->getType() << std::endl;
                            delete c2;
                            return *out;
                        }
                    } else {
                        auto *b = dynamic_cast<ValueNode *>(a);
                        auto c = b->getValue(c2);
                        if (c != nullptr) {
                            delete c;
                        }
                    }
                }

                delete c2;
                return nullptr;
            }

            void BlockNode::addStatement(Node *n) {
                this->nodes.push_back(n);
            }
        }
    }
}