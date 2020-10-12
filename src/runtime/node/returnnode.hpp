#pragma once

#include "../node.hpp"
#include "executablenode.hpp"

namespace piper {
    namespace runtime {
        namespace node {
            class ReturnNode : public Node, public ExecutableNode {
                public:
                    ReturnNode(Node *node);

                    Type *getType(Context *context);
                    Variable *getValue(Context *context);
                    Variable *execute(Context *context, Variable **out);

                    std::string toString();

                    virtual ~ReturnNode() = default;
                protected:
                    Node *node;
            };
        }
    }
}