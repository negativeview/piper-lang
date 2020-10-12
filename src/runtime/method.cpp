#include "method.hpp"

#include "node/valuenode.hpp"

#include <iostream>

namespace piper {
    namespace runtime {
        Method::Method(
            const std::string &method_name
        ) {
            this->method_name = method_name;
        }

        Method::Method(
            const std::string &type_name,
            const std::string &method_name
        ) {
            this->method_name = method_name;
            this->type_name = type_name;
        }

        Variable *Method::call(
            Context *context,
            const std::map<std::string, Node *> &arguments
        ) {
            std::cout << "Calling method " << this->method_name << ", arguments: " << std::endl;

            std::cout << "\tPrior:" << std::endl;
            context->debug();
            
            std::cout << "Setting variables..." << std::endl;
            auto c2 = new Context(context, false);
            for (auto a : arguments) {
                auto *vn = dynamic_cast<piper::runtime::node::ValueNode *>(a.second);
                if (vn == nullptr) {
                    // TODO: Error
                } else {
                    c2->setVariableNamed(a.first, vn->getValue(context));
                }
            }

            std::cout << "\tNew:" << std::endl;
            c2->debug();

            Variable *out;
            this->block.execute(c2, &out);
            std::cout << "Method returning: " << out << ":" << out->getType() << std::endl;
            return out;
        }

        piper::runtime::node::BlockNode *Method::getBlock() {
            return &(this->block);
        }

        void Method::addArgument(
            const std::string &name,
            Type *type
        ) {
            // TODO: Add detection of duplicates.

            std::cout << "Adding arg: " << name << ": " << type->toString() << std::endl;
            this->arg_list[name] = type;
        }

        std::map<std::string, piper::runtime::Type *> *Method::getArguments() {
            return &this->arg_list;
        }
    }
}