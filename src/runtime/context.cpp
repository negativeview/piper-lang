#include "context.hpp"
#include "runtime.hpp"

#include <iostream>
#include <memory>

namespace piper {
    namespace runtime {
        Context::Context(Runtime *runtime) {
            this->parent = nullptr;
            this->use_variables = false;
            this->runtime = runtime;
        }

        Context::Context(Context *parent, bool use_variables) {
            this->use_variables = use_variables;
            this->parent = parent;
            this->runtime = this->parent->runtime;
        }

        void Context::addFunction(
            FunctionType function_type,
            Type *type_a,
            Type *type_b,
            std::function<Variable *(Context *, Variable *, Variable *)> *f
        ) {
            if (this->functions.find(function_type) == this->functions.end()) {
                this->functions[function_type] = std::make_unique<
                    std::map<
                        Type *,
                        std::unique_ptr<
                            std::map<
                                Type *,
                                std::function<Variable *(Context *, Variable *, Variable *)> *
                            >
                        >
                    >
                >();
            }

            auto a = this->functions[function_type].get();

            if (a->find(type_a) == a->end()) {
                (*a)[type_a] = std::make_unique<
                    std::map<
                        Type *,
                        std::function<Variable *(Context *, Variable *, Variable *)> *
                    >
                >();
            }

            auto b = (*a)[type_a].get();
            (*b)[type_b] = f;
        }

        std::function<Variable *(Context *, Variable *, Variable *)> *Context::getFunction(FunctionType function_type, Type *type_a, Type *type_b) {
            if (this->functions.find(function_type) == this->functions.end()) {
                return nullptr;
            }

            auto a = this->functions[function_type].get();

            if (a->find(type_a) == a->end()) {
                return nullptr;
            }

            auto b = (*a)[type_a].get();

            if (b->find(type_b) == b->end()) {
                return nullptr;
            }

            return (*b)[type_b];
        }

        void Context::addComparisonFunctionLessThan(Type *t1, Type *t2, std::function<Variable *(Context *, Variable *, Variable *)> *f) {
            this->addFunction(FunctionType::LESS_THAN, t1, t2, f);
        }

        void Context::addBinaryFunctionMinus(Type *t1, Type *t2, std::function<Variable *(Context *, Variable *, Variable *)> *f) {
            this->addFunction(FunctionType::MINUS, t1, t2, f);
        }

        void Context::addBinaryFunctionAdd(Type *t1, Type *t2, std::function<Variable *(Context *, Variable *, Variable *)> *f) {
            this->addFunction(FunctionType::ADD, t1, t2, f);
        }

        std::function<Variable *(Context *, Variable *, Variable *)> *Context::getComparisonFunctionLessThan(Type *t1, Type *t2) {
            return this->getFunction(FunctionType::LESS_THAN, t1, t2);
        }

        std::function<Variable *(Context *, Variable *, Variable *)> *Context::getBinaryFunctionMinus(Type *t1, Type *t2) {
            return this->getFunction(FunctionType::MINUS, t1, t2);
        }

        std::function<Variable *(Context *, Variable *, Variable *)> *Context::getBinaryFunctionAdd(Type *t1, Type *t2) {
            return this->getFunction(FunctionType::ADD, t1, t2);
        }

        Runtime *Context::getRuntime() {
            return this->runtime;
        }

        void Context::debug() {
            for (auto a: this->variables) {
                std::cout << a.first << ":" << a.second->toString() << std::endl;
            }
            if (this->parent != nullptr && this->use_variables) {
                this->parent->debug();
            }
        }

        Variable *Context::getVariableNamed(const std::string &name) {
            if (this->variables.find(name) != this->variables.end()) {
                return this->variables[name];
            }
            if (this->parent != nullptr && this->use_variables) {
                return this->parent->getVariableNamed(name);
            }
            return nullptr;
        }

        void Context::setVariableNamed(const std::string &name, Variable *v) {
            this->variables[name] = v;
        }
    }
}