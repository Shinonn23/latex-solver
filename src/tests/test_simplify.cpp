#include "../core/ast/symbol.h"
#include "../core/common/parser.h"
#include "../core/rewrite/simplify.h"
#include <cassert>
#include <iostream>

using namespace latex_solver;

int test_count = 0;
int passed_count = 0;

#define TEST(name)                                                             \
	test_count++;                                                              \
	std::cout << "Test " << test_count << ": " << name << "..." << std::endl

#define ASSERT_STR_EQ(actual, expected)                                        \
	if ((actual) != (expected)) {                                              \
		std::cerr << "  FAILED: Expected \"" << (expected) << "\" but got \""  \
				  << (actual) << "\"" << std::endl;                            \
		throw std::runtime_error("Assertion failed");                          \
	}

#define PASS()                                                                 \
	std::cout << "  PASSED" << std::endl;                                      \
	passed_count++

void test_simplify_add_zero() {
	TEST("Simplify x + 0 = x");
	Parser p("x + 0");
	auto expr = p.parse();
	auto simplified = Simplifier::simplify(*expr);
	ASSERT_STR_EQ(simplified->to_string(), "x");
	PASS();
}

void test_simplify_zero_add() {
	TEST("Simplify 0 + x = x");
	Parser p("0 + x");
	auto expr = p.parse();
	auto simplified = Simplifier::simplify(*expr);
	ASSERT_STR_EQ(simplified->to_string(), "x");
	PASS();
}

void test_simplify_sub_zero() {
	TEST("Simplify x - 0 = x");
	Parser p("x - 0");
	auto expr = p.parse();
	auto simplified = Simplifier::simplify(*expr);
	ASSERT_STR_EQ(simplified->to_string(), "x");
	PASS();
}

void test_simplify_mul_zero() {
	TEST("Simplify x * 0 = 0");
	Parser p("x * 0");
	auto expr = p.parse();
	auto simplified = Simplifier::simplify(*expr);
	ASSERT_STR_EQ(simplified->to_string(), "0");
	PASS();
}

void test_simplify_mul_one() {
	TEST("Simplify x * 1 = x");
	Parser p("x * 1");
	auto expr = p.parse();
	auto simplified = Simplifier::simplify(*expr);
	ASSERT_STR_EQ(simplified->to_string(), "x");
	PASS();
}

void test_simplify_one_mul() {
	TEST("Simplify 1 * x = x");
	Parser p("1 * x");
	auto expr = p.parse();
	auto simplified = Simplifier::simplify(*expr);
	ASSERT_STR_EQ(simplified->to_string(), "x");
	PASS();
}

void test_simplify_div_one() {
	TEST("Simplify x / 1 = x");
	Parser p("x / 1");
	auto expr = p.parse();
	auto simplified = Simplifier::simplify(*expr);
	ASSERT_STR_EQ(simplified->to_string(), "x");
	PASS();
}

void test_simplify_constant_folding() {
	TEST("Simplify constant folding: 2 + 3");
	Parser p("2 + 3");
	auto expr = p.parse();
	auto simplified = Simplifier::simplify(*expr);
	ASSERT_STR_EQ(simplified->to_string(), "5");
	PASS();
}

void test_simplify_sqrt_constant() {
	TEST("Simplify \\sqrt{16} = 4");
	Parser p("\\sqrt{16}");
	auto expr = p.parse();
	auto simplified = Simplifier::simplify(*expr);
	ASSERT_STR_EQ(simplified->to_string(), "4");
	PASS();
}

int main() {
	std::cout << "=== Simplification Tests ===" << std::endl << std::endl;

	try {
		test_simplify_add_zero();
		test_simplify_zero_add();
		test_simplify_sub_zero();
		test_simplify_mul_zero();
		test_simplify_mul_one();
		test_simplify_one_mul();
		test_simplify_div_one();
		test_simplify_constant_folding();
		test_simplify_sqrt_constant();

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
