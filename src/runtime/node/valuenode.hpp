#pragma once

namespace piper {
    namespace runtime {
        namespace node {
            class ValueNode {
                public:
                    virtual Type *getType(Context *context) = 0;
                    virtual Variable *getValue(Context *context) = 0;
            };
        }
    }
}