#pragma once

#include "namespace.hpp"

#include <map>

namespace piper {
    namespace runtime {
        class Runtime {
            public:
                Runtime();

                Namespace *addGetNamespace(const std::string &namespaceName);
                Type *getGlobalType(const std::string &name);

                ~Runtime() = default;
            protected:
                std::map<std::string, Namespace *> namespaces;
                std::map<std::string, Type *> global_types;
        };
    }
}