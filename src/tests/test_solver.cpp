#include "../core/ast/equation.h"
#include "../core/common/context.h"
#include "../core/common/parser.h"
#include "../core/solver/algebra/linear_solver.h"
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

void test_solve_simple() {
    TEST("Solve simple equation: x + 5 = 10");
    Parser  p("x + 5 = 10");
    auto    eq = p.parse_equation();
    Context ctx;
    double  solution = LinearSolver::solve(*eq, ctx);
    ASSERT_EQ(solution, 5.0);
    PASS();
}

void test_solve_multiplication() {
    TEST("Solve with multiplication: 2 * x = 10");
    Parser  p("2 \\times x = 10");
    auto    eq = p.parse_equation();
    Context ctx;
    double  solution = LinearSolver::solve(*eq, ctx);
    ASSERT_EQ(solution, 5.0);
    PASS();
}

void test_solve_complex() {
    TEST("Solve complex equation: 2 * x - 3 = 7");
    Parser  p("2 \\times x - 3 = 7");
    auto    eq = p.parse_equation();
    Context ctx;
    double  solution = LinearSolver::solve(*eq, ctx);
    ASSERT_EQ(solution, 5.0);
    PASS();
}

void test_solve_reversed() {
    TEST("Solve reversed equation: 10 = x + 5");
    Parser  p("10 = x + 5");
    auto    eq = p.parse_equation();
    Context ctx;
    double  solution = LinearSolver::solve(*eq, ctx);
    ASSERT_EQ(solution, 5.0);
    PASS();
}

void test_solve_with_division() {
    TEST("Solve with division: x / 2 = 5");
    Parser  p("x \\div 2 = 5");
    auto    eq = p.parse_equation();
    Context ctx;
    double  solution = LinearSolver::solve(*eq, ctx);
    ASSERT_EQ(solution, 10.0);
    PASS();
}

void test_solve_negative_solution() {
    TEST("Solve with negative solution: x + 10 = 5");
    Parser  p("x + 10 = 5");
    auto    eq = p.parse_equation();
    Context ctx;
    double  solution = LinearSolver::solve(*eq, ctx);
    ASSERT_EQ(solution, -5.0);
    PASS();
}

int main() {
    std::cout << "=== Equation Solver Tests ===" << std::endl << std::endl;

    try {
        test_solve_simple();
        test_solve_multiplication();
        test_solve_complex();
        test_solve_reversed();
        test_solve_with_division();
        test_solve_negative_solution();

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
