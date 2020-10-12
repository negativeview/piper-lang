#include "runtime.hpp"

#include "types/i32.hpp"

namespace piper {
    namespace runtime {
        Runtime::Runtime() {
            this->global_types["i32"] = new piper::runtime::types::TypeI32();
        }

        Namespace *Runtime::addGetNamespace(const std::string &namespaceName) {
            if (this->namespaces.find(namespaceName) == this->namespaces.end()) {
                auto ns = new Namespace();
                this->namespaces[namespaceName] = ns;
            }

            return this->namespaces[namespaceName];
        }

        Type *Runtime::getGlobalType(const std::string &name) {
            if (this->global_types.find(name) == this->global_types.end()) {
                return nullptr;
            }

            return this->global_types[name];
        }
    }
}