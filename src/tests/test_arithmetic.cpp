#include "../core/ast/number.h"
#include "../core/common/context.h"
#include "../core/common/evaluator.h"
#include "../core/common/parser.h"
#include <cassert>
#include <cmath>
#include <iostream>

using namespace latex_solver;

int test_count = 0;
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

void test_addition() {
	TEST("Basic addition: 2 + 3");
	Parser p("2 + 3");
	auto expr = p.parse();
	Context ctx;
	Evaluator eval(ctx);
	double result = eval.evaluate(*expr);
	ASSERT_EQ(result, 5.0);
	PASS();
}

void test_subtraction() {
	TEST("Basic subtraction: 10 - 3");
	Parser p("10 - 3");
	auto expr = p.parse();
	Context ctx;
	Evaluator eval(ctx);
	double result = eval.evaluate(*expr);
	ASSERT_EQ(result, 7.0);
	PASS();
}

void test_multiplication() {
	TEST("Basic multiplication: 4 \\times 5");
	Parser p("4 \\times 5");
	auto expr = p.parse();
	Context ctx;
	Evaluator eval(ctx);
	double result = eval.evaluate(*expr);
	ASSERT_EQ(result, 20.0);
	PASS();
}

void test_division() {
	TEST("Basic division: 15 \\div 3");
	Parser p("15 \\div 3");
	auto expr = p.parse();
	Context ctx;
	Evaluator eval(ctx);
	double result = eval.evaluate(*expr);
	ASSERT_EQ(result, 5.0);
	PASS();
}

void test_operator_precedence() {
	TEST("Operator precedence: 2 + 3 \\times 4");
	Parser p("2 + 3 \\times 4");
	auto expr = p.parse();
	Context ctx;
	Evaluator eval(ctx);
	double result = eval.evaluate(*expr);
	ASSERT_EQ(result, 14.0);
	PASS();
}

void test_parentheses() {
	TEST("Parentheses: (2 + 3) \\times 4");
	Parser p("(2 + 3) \\times 4");
	auto expr = p.parse();
	Context ctx;
	Evaluator eval(ctx);
	double result = eval.evaluate(*expr);
	ASSERT_EQ(result, 20.0);
	PASS();
}

void test_nested_parentheses() {
	TEST("Nested parentheses: ((2 + 3) \\times 4) \\div 5");
	Parser p("((2 + 3) \\times 4) \\div 5");
	auto expr = p.parse();
	Context ctx;
	Evaluator eval(ctx);
	double result = eval.evaluate(*expr);
	ASSERT_EQ(result, 4.0);
	PASS();
}

void test_unary_minus() {
	TEST("Unary minus: -5");
	Parser p("-5");
	auto expr = p.parse();
	Context ctx;
	Evaluator eval(ctx);
	double result = eval.evaluate(*expr);
	ASSERT_EQ(result, -5.0);
	PASS();
}

void test_unary_minus_expression() {
	TEST("Unary minus in expression: 5 \\times -5");
	Parser p("5 \\times -5");
	auto expr = p.parse();
	Context ctx;
	Evaluator eval(ctx);
	double result = eval.evaluate(*expr);
	ASSERT_EQ(result, -25.0);
	PASS();
}

void test_complex_expression() {
	TEST("Complex expression: 10 \\times (5 + 3)");
	Parser p("10 \\times (5 + 3)");
	auto expr = p.parse();
	Context ctx;
	Evaluator eval(ctx);
	double result = eval.evaluate(*expr);
	ASSERT_EQ(result, 80.0);
	PASS();
}

int main() {
	std::cout << "=== Arithmetic Tests ===" << std::endl << std::endl;

	try {
		test_addition();
		test_subtraction();
		test_multiplication();
		test_division();
		test_operator_precedence();
		test_parentheses();
		test_nested_parentheses();
		test_unary_minus();
		test_unary_minus_expression();
		test_complex_expression();

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
