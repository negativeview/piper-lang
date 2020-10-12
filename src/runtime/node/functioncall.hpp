#pragma once

#include "../method.hpp"
#include "../node.hpp"
#include "../variable.hpp"
#include "valuenode.hpp"

#include <map>

namespace piper {
    namespace runtime {
        namespace node {
            class FunctionCall : public Node, public ValueNode {
                public:
                    FunctionCall(Method *m);

                    Type *getType(Context *context);
                    Variable *getValue(Context *context);

                    void addParameter(const std::string &key, Node *value);
                    std::string toString();

                    virtual ~FunctionCall() = default;
                protected:
                    Method *method;
                    std::map<std::string, Node *> parameters;
            };
        }
    }
}