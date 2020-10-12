#pragma once

#include "../context.hpp"

namespace piper {
    namespace runtime {
        namespace node {
            class BooleanNode {
                public:
                    virtual bool getBoolean(Context *context) = 0;
            };
        }
    }
}