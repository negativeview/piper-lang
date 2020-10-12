#pragma once

#include "../method.hpp"
#include "../node.hpp"
#include "../variable.hpp"
#include "valuenode.hpp"

#include <map>

namespace piper {
    namespace runtime {
        namespace node {
            class AddNode : public Node, public ValueNode {
                public:
                    AddNode(Node *left, Node *right);

                    Type *getType(Context *context);
                    Variable *getValue(Context *context);

                    std::string toString();

                    virtual ~AddNode() = default;
                protected:
                    Node *left;
                    Node *right;
            };
        }
    }
}