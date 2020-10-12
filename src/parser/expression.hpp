#pragma once

#include <string>

namespace piper {
    namespace parser {
        class Expression {
            public:
                virtual std::string toString() = 0;
                // TODO: Get output type.

                virtual ~Expression() {
                    
                }
            protected:
        };
    }
}