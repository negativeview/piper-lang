#pragma once

#include "../node.hpp"
#include "executablenode.hpp"
#include "booleannode.hpp"
#include "valuenode.hpp"

namespace piper {
    namespace runtime {
        namespace node {
            class IfNode : public Node, public ValueNode {
                public:
                    IfNode(
                        Node *conditional,
                        ExecutableNode *trueBranch,
                        ExecutableNode *elseBranch
                    );

                    Type *getType(Context *context);
                    Variable *getValue(Context *context);

                    std::string toString();

                    virtual ~IfNode() = default;
                protected:
                    Node *conditionalNode;
                    ExecutableNode *trueNode;
                    ExecutableNode *falseNode;
            };
        }
    }
}