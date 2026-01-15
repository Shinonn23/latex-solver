#include "../core/ast/symbol.h"
#include "../core/common/context.h"
#include "../core/common/error.h"
#include "../core/eval/evaluator.h"
#include "../core/parser/parser.h"
#include <cassert>
#include <cmath>
#include <iostream>

using namespace latex_solver;

int test_count   = 0;
int passed_count = 0;

#define TEST(name)                                                             \
    test_count++;                                                              \
    std::cout << "Test " << test_count << ": " << name << "..." << std::endl

#define ASSERT_EQ(actual, expected)                                            \
    if (std::abs((actual) - (expected)) > 1e-10) {                             \
        std::cerr << "  FAILED: Expected " << (expected) << " but got "        \
                  << (actual) << std::endl;                                    \
        throw std::runtime_error("Assertion failed");                          \
    }

#define PASS()                                                                 \
    std::cout << "  PASSED" << std::endl;                                      \
    passed_count++

void test_single_variable() {
    TEST("Single variable: x with x=10");
    Parser  p("x");
    auto    expr = p.parse();
    Context ctx;
    ctx.set("x", 10.0);
    Evaluator eval(ctx);
    double    result = eval.evaluate(*expr);
    ASSERT_EQ(result, 10.0);
    PASS();
}

void test_variable_in_expression() {
    TEST("Variable in expression: x * 2 + 5 with x=10");
    Parser  p("x \\times 2 + 5");
    auto    expr = p.parse();
    Context ctx;
    ctx.set("x", 10.0);
    Evaluator eval(ctx);
    double    result = eval.evaluate(*expr);
    ASSERT_EQ(result, 25.0);
    PASS();
}

void test_multiple_variables() {
    TEST("Multiple variables: x + y with x=3, y=7");
    Parser  p("x + y");
    auto    expr = p.parse();
    Context ctx;
    ctx.set("x", 3.0);
    ctx.set("y", 7.0);
    Evaluator eval(ctx);
    double    result = eval.evaluate(*expr);
    ASSERT_EQ(result, 10.0);
    PASS();
}

void test_undefined_variable() {
    TEST("Undefined variable error handling");
    Parser    p("x");
    auto      expr = p.parse();
    Context   ctx;
    Evaluator eval(ctx);
    try {
        eval.evaluate(*expr);
        std::cerr << "  FAILED: Should have thrown UndefinedVariableError"
                  << std::endl;
        throw std::runtime_error("Expected exception not thrown");
    } catch (const UndefinedVariableError&) {
        PASS();
    }
}

int main() {
    std::cout << "=== Variable Tests ===" << std::endl << std::endl;

    try {
        test_single_variable();
        test_variable_in_expression();
        test_multiple_variables();
        test_undefined_variable();

        std::cout << "\nAll " << passed_count << "/" << test_count
                  << " tests PASSED!" << std::endl;
        return 0;

    } catch (const std::exception& e) {
        std::cerr << "\nTest suite FAILED: " << e.what() << std::endl;
        std::cerr << "Passed: " << passed_count << "/" << test_count
                  << std::endl;
        return 1;
    }
}
