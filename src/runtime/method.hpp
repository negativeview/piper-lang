#pragma once

#include "context.hpp"
#include "type.hpp"
#include "variable.hpp"
#include "./node/blocknode.hpp"

#include <map>
#include <string>

namespace piper {
    namespace runtime {
        class Method {
            public:
                Method(
                    const std::string &method_name
                );
                Method(
                    const std::string &type_name,
                    const std::string &method_name
                );

                void addArgument(const std::string &name, Type *type);
                Variable *call(
                    Context *context,
                    const std::map<std::string, Node *> &arguments
                );
                piper::runtime::node::BlockNode *getBlock();
                std::map<std::string, piper::runtime::Type *> *getArguments();
            protected:
                std::string type_name;
                std::string method_name;
                std::map<std::string, Type *> arg_list;
                piper::runtime::node::BlockNode block;
        };
    }
}