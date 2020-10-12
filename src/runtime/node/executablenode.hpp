#pragma once

namespace piper {
    namespace runtime {
        namespace node {
            class ExecutableNode {
                public:
                    virtual Variable *execute(Context *context, Variable **out) = 0;
            };
        }
    }
}