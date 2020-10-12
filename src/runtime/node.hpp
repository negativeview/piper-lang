#pragma once

#include "context.hpp"
#include "type.hpp"
#include "variable.hpp"

namespace piper {
    namespace runtime {
        class Node {
            public:
                virtual std::string toString() = 0;

                virtual ~Node() = default;
        };
    }
}