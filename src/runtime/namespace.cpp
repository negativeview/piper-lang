#include "namespace.hpp"

namespace piper {
    namespace runtime {
        Struct *Namespace::createStructShell(const std::string &name) {
            if (this->structs.find(name) != this->structs.end()) {
                // This is weird. We HAVE a struct. So it feels weird to return
                // nullptr. What we really want is a good way to say, "this
                // is attempting to duplicate."
                return nullptr;
            }

            auto st = new Struct();
            this->structs[name] = st;
            return st;
        }

        Method *Namespace::createMethod(const std::string &name) {
            if (this->methods.find(name) != this->methods.end()) {
                return nullptr;
            }

            auto m = new Method(name);
            this->methods[name] = m;
            return m;
        }
    }
}