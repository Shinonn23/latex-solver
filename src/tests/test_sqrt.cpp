#include "../core/ast/function.h"
#include "../core/ast/number.h"
#include "../core/common/context.h"
#include "../core/common/evaluator.h"
#include "../core/common/parser.h"
#include "../core/rewrite/simplify.h"
#include <cassert>
#include <cmath>
#include <iostream>

using namespace latex_solver;

void test_sqrt_parsing() {
	std::cout << "Test: Parsing \\sqrt{16}..." << std::endl;
	Parser p("\\sqrt{16}");
	auto expr = p.parse();
	std::cout << "  Parsed: " << expr->to_string() << std::endl;
	assert(expr->to_string() == "sqrt(16)");
	std::cout << "  PASSED" << std::endl;
}

void test_sqrt_evaluation() {
	std::cout << "Test: Evaluating \\sqrt{16}..." << std::endl;
	Parser p("\\sqrt{16}");
	auto expr = p.parse();
	Context ctx;
	Evaluator eval(ctx);
	double result = eval.evaluate(*expr);
	std::cout << "  Result: " << result << std::endl;
	assert(std::abs(result - 4.0) < 1e-10);
	std::cout << "  PASSED" << std::endl;
}

void test_sqrt_simplification() {
	std::cout << "Test: Simplifying \\sqrt{16}..." << std::endl;
	Parser p("\\sqrt{16}");
	auto expr = p.parse();
	auto simplified = Simplifier::simplify(*expr);
	std::cout << "  Simplified: " << simplified->to_string() << std::endl;
	assert(simplified->to_string() == "4");
	std::cout << "  PASSED" << std::endl;
}

void test_sqrt_in_expression() {
	std::cout << "Test: Evaluating \\sqrt{9} + 3..." << std::endl;
	Parser p("\\sqrt{9} + 3");
	auto expr = p.parse();
	Context ctx;
	Evaluator eval(ctx);
	double result = eval.evaluate(*expr);
	std::cout << "  Result: " << result << std::endl;
	assert(std::abs(result - 6.0) < 1e-10);
	std::cout << "  PASSED" << std::endl;
}

void test_sqrt_nested() {
	std::cout << "Test: Evaluating 2 * \\sqrt{25}..." << std::endl;
	Parser p("2 * \\sqrt{25}");
	auto expr = p.parse();
	Context ctx;
	Evaluator eval(ctx);
	double result = eval.evaluate(*expr);
	std::cout << "  Result: " << result << std::endl;
	assert(std::abs(result - 10.0) < 1e-10);
	std::cout << "  PASSED" << std::endl;
}

int main() {
	std::cout << "=== Testing \\sqrt{} Support ===" << std::endl << std::endl;

	try {
		test_sqrt_parsing();
		test_sqrt_evaluation();
		test_sqrt_simplification();
		test_sqrt_in_expression();
		test_sqrt_nested();

		std::cout << std::endl << "All tests PASSED!" << std::endl;
		return 0;
	} catch (const std::exception &e) {
		std::cerr << "Test FAILED: " << e.what() << std::endl;
		return 1;
	}
}
