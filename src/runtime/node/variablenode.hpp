#pragma once

#include "../node.hpp"
#include "../variable.hpp"
#include "valuenode.hpp"

namespace piper {
    namespace runtime {
        namespace node {
            class VariableNode : public Node, public ValueNode {
                public:
                    VariableNode(Variable *variable);
                    VariableNode(const std::string &variable_name);

                    Type *getType(Context *context);
                    Variable *getValue(Context *context);

                    std::string toString();

                    virtual ~VariableNode() = default;
                protected:
                    Variable *variable;
                    std::string variable_name;
            };
        }
    }
}