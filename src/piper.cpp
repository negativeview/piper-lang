#include "logger/logger.hpp"
#include "parser/parser.hpp"
#include "runtime/context.hpp"
#include "runtime/namespace.hpp"
#include "runtime/node/addnode.hpp"
#include "runtime/node/assignmentnode.hpp"
#include "runtime/node/blocknode.hpp"
#include "runtime/node/functioncall.hpp"
#include "runtime/node/ifnode.hpp"
#include "runtime/node/lessthan.hpp"
#include "runtime/node/minusnode.hpp"
#include "runtime/node/returnnode.hpp"
#include "runtime/node/variablenode.hpp"
#include "runtime/returnexception.hpp"
#include "runtime/runtime.hpp"
#include "runtime/variable.hpp"
#include "tokenizer/token.hpp"
#include "tokenizer/tokenizer.hpp"

#include <chrono>
#include <functional>
#include <iostream>
#include <map>

piper::runtime::Variable *lt_i32_i32(piper::runtime::Context *context, piper::runtime::Variable *a, piper::runtime::Variable *b) {
    auto aa = static_cast<uint32_t *>(a->getData());
    auto bb = static_cast<uint32_t *>(b->getData());

    // TODO: Make this the boolean type
    return context->getRuntime()->getGlobalType("i32")->getVariableFromString(
        std::to_string(
            *aa < *bb
        )
    );
}

piper::runtime::Variable *minus_i32_i32(piper::runtime::Context *context, piper::runtime::Variable *a, piper::runtime::Variable *b) {
    auto aa = static_cast<uint32_t *>(a->getData());
    auto bb = static_cast<uint32_t *>(b->getData());

    return context->getRuntime()->getGlobalType("i32")->getVariableFromString(
        std::to_string(
            *aa - *bb
        )
    );
}

piper::runtime::Variable *add_i32_i32(piper::runtime::Context *context, piper::runtime::Variable *a, piper::runtime::Variable *b) {
    auto aa = static_cast<uint32_t *>(a->getData());
    auto bb = static_cast<uint32_t *>(b->getData());

    return context->getRuntime()->getGlobalType("i32")->getVariableFromString(
        std::to_string(
            *aa + *bb
        )
    );
}

int main(int argc, char *argv[]) {
    piper::logger::Logger logger;

    piper::tokenizer::Tokenizer tokenizer(&logger);

    if (argc > 1) {
        // auto time_a = std::chrono::steady_clock::now();

        // std::vector<piper::tokenizer::Token *> *tokens = tokenizer.tokenizeAsync(argv[1]);

        // auto time_b = std::chrono::steady_clock::now();

        if (logger.getHadError()) {
            exit(-1);
        }

        piper::runtime::Runtime runtime;

        piper::runtime::Context baseContext(&runtime);

        std::function<piper::runtime::Variable *(piper::runtime::Context *, piper::runtime::Variable *, piper::runtime::Variable *)> lt_func = lt_i32_i32;
        baseContext.addComparisonFunctionLessThan(runtime.getGlobalType("i32"), runtime.getGlobalType("i32"), &lt_func);

        std::function<piper::runtime::Variable *(piper::runtime::Context *, piper::runtime::Variable *, piper::runtime::Variable *)> minus_func = minus_i32_i32;
        baseContext.addBinaryFunctionMinus(runtime.getGlobalType("i32"), runtime.getGlobalType("i32"), &minus_func);

        std::function<piper::runtime::Variable *(piper::runtime::Context *, piper::runtime::Variable *, piper::runtime::Variable *)> add_func = add_i32_i32;
        baseContext.addBinaryFunctionAdd(runtime.getGlobalType("i32"), runtime.getGlobalType("i32"), &add_func);

        piper::runtime::Namespace *ns = runtime.addGetNamespace("default");
        piper::runtime::Method *fib = ns->createMethod("fib");
        fib->addArgument("a", runtime.getGlobalType("i32"));

        std::map<std::string, piper::runtime::Node *> methodArguments;

        piper::runtime::Variable *va = runtime.getGlobalType("i32")->getVariableFromString("45");
        methodArguments["a"] = new piper::runtime::node::VariableNode(va);

        auto vna = new piper::runtime::node::VariableNode("a");

        piper::runtime::Variable *vb = runtime.getGlobalType("i32")->getVariableFromString("2");
        auto vnb = new piper::runtime::node::VariableNode(vb);

        auto lt = new piper::runtime::node::LessThan(
            &runtime,
            vna,
            vnb
        );

        auto bn1 = new piper::runtime::node::BlockNode();
        auto rn = new piper::runtime::node::ReturnNode(
            new piper::runtime::node::VariableNode("a")
        );
        bn1->addStatement(rn);

        auto bn2 = new piper::runtime::node::BlockNode();
        auto fc1 = new piper::runtime::node::FunctionCall(fib);
        fc1->addParameter(
            "a",
            new piper::runtime::node::MinusNode(
                new piper::runtime::node::VariableNode("a"),
                new piper::runtime::node::VariableNode(
                    runtime.getGlobalType("i32")->getVariableFromString("1")
                )
            )
        );
        auto fc2 = new piper::runtime::node::FunctionCall(fib);
        fc2->addParameter(
            "a",
            new piper::runtime::node::MinusNode(
                new piper::runtime::node::VariableNode("a"),
                new piper::runtime::node::VariableNode(
                    runtime.getGlobalType("i32")->getVariableFromString("2")
                )
            )
        );

        bn2->addStatement(
            new piper::runtime::node::ReturnNode(
                new piper::runtime::node::AddNode(
                    fc1,
                    fc2
                )
            )
        );

        auto ifnode = new piper::runtime::node::IfNode(
            lt,
            bn1,
            bn2
        );
        fib->getBlock()->addStatement(ifnode);

        try {
            auto o = fib->call(&baseContext, methodArguments);
        } catch (piper::runtime::ReturnException &e) {
            std::cout << "Final result: " << e.getVariable()->toString() << std::endl;
        }

        // auto time_c = std::chrono::steady_clock::now();

        // piper::parser::Parser parser(&logger, &runtime);
        // parser.parse(tokens);

        // auto time_d = std::chrono::steady_clock::now();

        // std::cout << "Tokenizing: " << (time_b - time_a).count() << "ns" << std::endl;
        // std::cout << "Parsing:    " << (time_d - time_c).count() << "ns" << std::endl;
    }
}