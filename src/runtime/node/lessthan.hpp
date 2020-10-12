#pragma once

#include "../node.hpp"
#include "../context.hpp"
#include "../runtime.hpp"
#include "booleannode.hpp"
#include "valuenode.hpp"

namespace piper {
    namespace runtime {
        namespace node {
            class LessThan : public Node, public ValueNode {
                public:
                    LessThan(Runtime *runtime, Node *left, Node *right);

                    Type *getType(Context *context);
                    Variable *getValue(Context *context);

                    std::string toString();

                    ~LessThan() = default;
                protected:
                    Node *left;
                    Node *right;
                    Runtime *runtime;
            };
        }
    }
}