#pragma once

#include "../node.hpp"
#include "../context.hpp"
#include "executablenode.hpp"

#include <vector>

namespace piper {
    namespace runtime {
        namespace node {
            class BlockNode : public Node, public ExecutableNode {
                public:
                    BlockNode() = default;

                    Variable *execute(Context *context, Variable **out);
                    std::string toString();

                    void addStatement(Node *node);

                    ~BlockNode() = default;
                protected:
                    std::vector<Node *> nodes;
            };
        }
    }
}