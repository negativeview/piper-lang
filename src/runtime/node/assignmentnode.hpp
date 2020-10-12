#pragma once

#include "../node.hpp"

namespace piper {
    namespace runtime {
        namespace node {
            class AssignmentNode : public Node {
                public:
                    AssignmentNode() = default;

                    std::string toString();

                    ~AssignmentNode() = default;
                protected:
            };
        }
    }
}