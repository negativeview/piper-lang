#pragma once

#include "struct.hpp"
#include "method.hpp"

#include <map>
#include <string>

namespace piper {
    namespace runtime {
        class Namespace {
            public:
                Namespace() = default;

                Struct *createStructShell(const std::string &name);
                Method *createMethod(const std::string &name);

                ~Namespace() = default;
            protected:
                std::map<std::string, Struct *> structs;
                std::map<std::string, Method *> methods;
        };
    }
}