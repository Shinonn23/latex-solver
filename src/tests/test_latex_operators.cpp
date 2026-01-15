#include "../core/ast/number.h"
#include "../core/common/context.h"
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

void test_latex_times() {
    TEST("LaTeX \\times operator: 3 \\times 4");
    Parser    p("3 \\times 4");
    auto      expr = p.parse();
    Context   ctx;
    Evaluator eval(ctx);
    double    result = eval.evaluate(*expr);
    ASSERT_EQ(result, 12.0);
    PASS();
}

void test_latex_div() {
    TEST("LaTeX \\div operator: 20 \\div 4");
    Parser    p("20 \\div 4");
    auto      expr = p.parse();
    Context   ctx;
    Evaluator eval(ctx);
    double    result = eval.evaluate(*expr);
    ASSERT_EQ(result, 5.0);
    PASS();
}

void test_mixed_operators() {
    TEST("Mixed operators: 2 + 3 \\times 4");
    Parser    p("2 + 3 \\times 4");
    auto      expr = p.parse();
    Context   ctx;
    Evaluator eval(ctx);
    double    result = eval.evaluate(*expr);
    ASSERT_EQ(result, 14.0);
    PASS();
}

int main() {
    std::cout << "=== LaTeX Operators Tests ===" << std::endl << std::endl;

    try {
        test_latex_times();
        test_latex_div();
        test_mixed_operators();

        std::cout << "\nAll " << passed_count << "/" << test_count
                  << " tests PASSED!" << std::endl;
        return 0;

    } catch (const std::exception &e) {
        std::cerr << "\nTest suite FAILED: " << e.what() << std::endl;
        std::cerr << "Passed: " << passed_count << "/" << test_count
                  << std::endl;
        return 1;
    }
}
