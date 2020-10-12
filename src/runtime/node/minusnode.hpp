#pragma once

#include "../method.hpp"
#include "../node.hpp"
#include "../variable.hpp"
#include "valuenode.hpp"

#include <map>

namespace piper {
    namespace runtime {
        namespace node {
            class MinusNode : public Node, public ValueNode {
                public:
                    MinusNode(Node *left, Node *right);

                    Type *getType(Context *context);
                    Variable *getValue(Context *context);

                    std::string toString();

                    virtual ~MinusNode() = default;
                protected:
                    Node *left;
                    Node *right;
            };
        }
    }
}