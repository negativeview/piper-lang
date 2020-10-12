#pragma once

#include <string>

namespace piper {
    namespace runtime {
        class Type;

        class Variable {
            public:
                Variable(Type *type);
                
                void setDataPtr(void *ptr);
                std::string toString();
                Type *getType();
                void *getData();
                bool isTruthy();

                ~Variable() = default;
            protected:
                Type *type;
                void *ptr;
        };
    }
}