#pragma once

#include "type.hpp"
#include "variable.hpp"

#include <functional>
#include <map>
#include <memory>

namespace piper {
    namespace runtime {
        class Runtime;

        enum FunctionType {
            LESS_THAN,
            MINUS,
            ADD
        };

        class Context {
            public:
                Context(Context *parent, bool use_variables);
                Context(Runtime *runtime);

                void addComparisonFunctionLessThan(Type *, Type *, std::function<Variable *(Context *, Variable *, Variable *)> *f);
                std::function<Variable *(Context *, Variable *, Variable *)> *getComparisonFunctionLessThan(Type *, Type *);

                void addBinaryFunctionMinus(Type *, Type *, std::function<Variable *(Context *, Variable *, Variable *)> *f);
                std::function<Variable *(Context *, Variable *, Variable *)> *getBinaryFunctionMinus(Type *, Type *);

                void addBinaryFunctionAdd(Type *, Type *, std::function<Variable *(Context *, Variable *, Variable *)> *f);
                std::function<Variable *(Context *, Variable *, Variable *)> *getBinaryFunctionAdd(Type *, Type *);

                Variable *getVariableNamed(const std::string &name);
                Variable *addVariableNamed(std::string name);
                void setVariableNamed(const std::string &name, Variable *v);
                void debug();
                Runtime *getRuntime();

                ~Context() = default;
            protected:
                void addFunction(
                    FunctionType function_type,
                    Type *type_a,
                    Type *type_b,
                    std::function<Variable *(Context *, Variable *, Variable *)> *f
                );

                std::function<Variable *(Context *, Variable *, Variable *)> *getFunction(
                    FunctionType function_type,
                    Type *type_a,
                    Type *type_b
                );

                bool use_variables;
                Context *parent;
                Runtime *runtime;
                std::map<
                    FunctionType,
                    std::unique_ptr<
                        std::map<
                            Type *,
                            std::unique_ptr<
                                std::map<
                                    Type *,
                                    std::function<
                                        Variable *(Context *c, Variable *, Variable *)
                                    > *
                                >
                            >
                        >
                    >
                > functions;

                // std::map<Type *, std::map<Type *, std::function<bool(Variable *, Variable *)> *> *> comparison_functions_less_than;
                // std::map<Type *, std::map<Type *, std::function<Variable *(Runtime *, Variable *, Variable *)> *> *> binary_functions_minus;
                // std::map<Type *, std::map<Type *, std::function<Variable *(Runtime *, Variable *, Variable *)> *> *> binary_functions_add;
                std::map<std::string, Variable *> variables;
        };
    }
}