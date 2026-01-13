#include "../core/common/error.h"
#include "../core/common/evaluator.h"
#include "../core/common/parser.h"
#include <iostream>

using namespace latex_solver;

int test_count = 0;
int passed_count = 0;

#define TEST(name)                                                             \
	test_count++;                                                              \
	std::cout << "Test " << test_count << ": " << name << "..." << std::endl

#define PASS()                                                                 \
	std::cout << "  PASSED" << std::endl;                                      \
	passed_count++

void test_division_by_zero() {
	TEST("Division by zero error handling");
	Parser p("5 / 0");
	auto expr = p.parse();
	Context ctx;
	Evaluator eval(ctx);
	try {
		eval.evaluate(*expr);
		std::cerr << "  FAILED: Should have thrown DivisionByZeroError"
				  << std::endl;
		throw std::runtime_error("Expected exception not thrown");
	} catch (const DivisionByZeroError &) {
		PASS();
	}
}

void test_invalid_syntax() {
	TEST("Invalid syntax error handling");
	try {
		Parser p("2 + + 3");
		auto expr = p.parse();
		std::cerr << "  FAILED: Should have thrown ParseError" << std::endl;
		throw std::runtime_error("Expected exception not thrown");
	} catch (const ParseError &) {
		PASS();
	}
}

void test_missing_closing_paren() {
	TEST("Missing closing parenthesis error handling");
	try {
		Parser p("(2 + 3");
		auto expr = p.parse();
		std::cerr << "  FAILED: Should have thrown ParseError" << std::endl;
		throw std::runtime_error("Expected exception not thrown");
	} catch (const ParseError &) {
		PASS();
	}
}

void test_missing_closing_brace() {
	TEST("Missing closing brace in sqrt error handling");
	try {
		Parser p("\\sqrt{16");
		auto expr = p.parse();
		std::cerr << "  FAILED: Should have thrown ParseError" << std::endl;
		throw std::runtime_error("Expected exception not thrown");
	} catch (const ParseError &) {
		PASS();
	}
}

void test_sqrt_negative() {
	TEST("sqrt of negative number error handling");
	Parser p("\\sqrt{-4}");
	auto expr = p.parse();
	Context ctx;
	Evaluator eval(ctx);
	try {
		eval.evaluate(*expr);
		std::cerr << "  FAILED: Should have thrown error for negative sqrt"
				  << std::endl;
		throw std::runtime_error("Expected exception not thrown");
	} catch (const std::runtime_error &) {
		PASS();
	}
}

int main() {
	std::cout << "=== Error Handling Tests ===" << std::endl << std::endl;

	try {
		test_division_by_zero();
		test_invalid_syntax();
		test_missing_closing_paren();
		test_missing_closing_brace();
		test_sqrt_negative();

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
