#include "../core/common/error.h"
#include "../core/parser/parser.h"
#include <cassert>
#include <iostream>
#include <string>

using namespace latex_solver;

int test_count   = 0;
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

// ==================== Number Parsing Tests ====================

void test_parse_integer() {
    TEST("Parse integer: 42");
    Parser p("42");
    auto   expr = p.parse();
    ASSERT_STR_EQ(expr->to_string(), "42");
    PASS();
}

void test_parse_float() {
    TEST("Parse float: 3.14");
    Parser p("3.14");
    auto   expr = p.parse();
    ASSERT_STR_EQ(expr->to_string(), "3.14");
    PASS();
}

void test_parse_zero() {
    TEST("Parse zero: 0");
    Parser p("0");
    auto   expr = p.parse();
    ASSERT_STR_EQ(expr->to_string(), "0");
    PASS();
}

// ==================== Variable Parsing Tests ====================

void test_parse_single_variable() {
    TEST("Parse single variable: x");
    Parser p("x");
    auto   expr = p.parse();
    ASSERT_STR_EQ(expr->to_string(), "x");
    PASS();
}

void test_parse_multi_char_variable() {
    TEST("Parse multi-character variable: var_name123");
    Parser p("var_name123");
    auto   expr = p.parse();
    ASSERT_STR_EQ(expr->to_string(), "var_name123");
    PASS();
}

// ==================== Binary Operation Parsing Tests ====================

void test_parse_addition() {
    TEST("Parse addition: 2 + 3");
    Parser p("2 + 3");
    auto   expr = p.parse();
    ASSERT_STR_EQ(expr->to_string(), "(2 + 3)");
    PASS();
}

void test_parse_subtraction() {
    TEST("Parse subtraction: 5 - 2");
    Parser p("5 - 2");
    auto   expr = p.parse();
    ASSERT_STR_EQ(expr->to_string(), "(5 - 2)");
    PASS();
}

void test_parse_multiplication() {
    TEST("Parse multiplication: 3 \\times 4");
    Parser p("3 \\times 4");
    auto   expr = p.parse();
    ASSERT_STR_EQ(expr->to_string(), "(3 * 4)");
    PASS();
}

void test_parse_division() {
    TEST("Parse division: 8 \\div 2");
    Parser p("8 \\div 2");
    auto   expr = p.parse();
    ASSERT_STR_EQ(expr->to_string(), "(8 / 2)");
    PASS();
}

void test_parse_latex_times() {
    TEST("Parse LaTeX times: 3 \\times 4");
    Parser p("3 \\times 4");
    auto   expr = p.parse();
    ASSERT_STR_EQ(expr->to_string(), "(3 * 4)");
    PASS();
}

void test_parse_latex_div() {
    TEST("Parse LaTeX div: 8 \\div 2");
    Parser p("8 \\div 2");
    auto   expr = p.parse();
    ASSERT_STR_EQ(expr->to_string(), "(8 / 2)");
    PASS();
}

// ==================== Precedence Tests ====================

void test_parse_precedence_mul_add() {
    TEST("Parse precedence: 2 + 3 \\times 4");
    Parser p("2 + 3 \\times 4");
    auto   expr = p.parse();
    ASSERT_STR_EQ(expr->to_string(), "(2 + (3 * 4))");
    PASS();
}

void test_parse_precedence_add_mul() {
    TEST("Parse precedence: 2 \\times 3 + 4");
    Parser p("2 \\times 3 + 4");
    auto   expr = p.parse();
    ASSERT_STR_EQ(expr->to_string(), "((2 * 3) + 4)");
    PASS();
}

void test_parse_associativity_left() {
    TEST("Parse left associativity: 10 - 3 - 2");
    Parser p("10 - 3 - 2");
    auto   expr = p.parse();
    ASSERT_STR_EQ(expr->to_string(), "((10 - 3) - 2)");
    PASS();
}

void test_parse_complex_precedence() {
    TEST("Parse complex precedence: 2 + 3 \\times 4 - 5 \\div 2");
    Parser p("2 + 3 \\times 4 - 5 \\div 2");
    auto   expr = p.parse();
    ASSERT_STR_EQ(expr->to_string(), "((2 + (3 * 4)) - (5 / 2))");
    PASS();
}

// ==================== Parentheses Tests ====================

void test_parse_simple_parentheses() {
    TEST("Parse simple parentheses: (2 + 3)");
    Parser p("(2 + 3)");
    auto   expr = p.parse();
    ASSERT_STR_EQ(expr->to_string(), "(2 + 3)");
    PASS();
}

void test_parse_parentheses_precedence() {
    TEST("Parse parentheses override precedence: (2 + 3) \\times 4");
    Parser p("(2 + 3) \\times 4");
    auto   expr = p.parse();
    ASSERT_STR_EQ(expr->to_string(), "((2 + 3) * 4)");
    PASS();
}

void test_parse_nested_parentheses() {
    TEST("Parse nested parentheses: ((2 + 3) \\times 4)");
    Parser p("((2 + 3) \\times 4)");
    auto   expr = p.parse();
    ASSERT_STR_EQ(expr->to_string(), "((2 + 3) * 4)");
    PASS();
}

void test_parse_multiple_parentheses() {
    TEST("Parse multiple parentheses: (2 + 3) \\times (4 + 5)");
    Parser p("(2 + 3) \\times (4 + 5)");
    auto   expr = p.parse();
    ASSERT_STR_EQ(expr->to_string(), "((2 + 3) * (4 + 5))");
    PASS();
}

// ==================== Unary Minus Tests ====================

void test_parse_unary_minus_number() {
    TEST("Parse unary minus with number: -5");
    Parser p("-5");
    auto   expr = p.parse();
    ASSERT_STR_EQ(expr->to_string(), "(0 - 5)");
    PASS();
}

void test_parse_unary_minus_variable() {
    TEST("Parse unary minus with variable: -x");
    Parser p("-x");
    auto   expr = p.parse();
    ASSERT_STR_EQ(expr->to_string(), "(0 - x)");
    PASS();
}

void test_parse_unary_minus_expression() {
    TEST("Parse unary minus with expression: -(2 + 3)");
    Parser p("-(2 + 3)");
    auto   expr = p.parse();
    ASSERT_STR_EQ(expr->to_string(), "(0 - (2 + 3))");
    PASS();
}

void test_parse_unary_minus_in_expression() {
    TEST("Parse unary minus in expression: 5 + -3");
    Parser p("5 + -3");
    auto   expr = p.parse();
    ASSERT_STR_EQ(expr->to_string(), "(5 + (0 - 3))");
    PASS();
}

// ==================== Function Parsing Tests ====================

void test_parse_sqrt_number() {
    TEST("Parse sqrt with number: \\sqrt{16}");
    Parser p("\\sqrt{16}");
    auto   expr = p.parse();
    ASSERT_STR_EQ(expr->to_string(), "sqrt(16)");
    PASS();
}

void test_parse_sqrt_variable() {
    TEST("Parse sqrt with variable: \\sqrt{x}");
    Parser p("\\sqrt{x}");
    auto   expr = p.parse();
    ASSERT_STR_EQ(expr->to_string(), "sqrt(x)");
    PASS();
}

void test_parse_sqrt_expression() {
    TEST("Parse sqrt with expression: \\sqrt{2 + 3}");
    Parser p("\\sqrt{2 + 3}");
    auto   expr = p.parse();
    ASSERT_STR_EQ(expr->to_string(), "sqrt((2 + 3))");
    PASS();
}

void test_parse_sqrt_in_expression() {
    TEST("Parse sqrt in expression: \\sqrt{9} + 3");
    Parser p("\\sqrt{9} + 3");
    auto   expr = p.parse();
    ASSERT_STR_EQ(expr->to_string(), "(sqrt(9) + 3)");
    PASS();
}

void test_parse_sqrt_multiplication() {
    TEST("Parse sqrt with multiplication: 2 \\times \\sqrt{25}");
    Parser p("2 \\times \\sqrt{25}");
    auto   expr = p.parse();
    ASSERT_STR_EQ(expr->to_string(), "(2 * sqrt(25))");
    PASS();
}

// ==================== Whitespace Tests ====================

void test_parse_no_whitespace() {
    TEST("Parse without whitespace: 2+3\\times4");
    Parser p("2+3\\times4");
    auto   expr = p.parse();
    ASSERT_STR_EQ(expr->to_string(), "(2 + (3 * 4))");
    PASS();
}

void test_parse_extra_whitespace() {
    TEST("Parse with extra whitespace:   2  +  3  *  4  ");
    Parser p("  2  +  3  \\times  4  ");
    auto   expr = p.parse();
    ASSERT_STR_EQ(expr->to_string(), "(2 + (3 * 4))");
    PASS();
}

// ==================== Equation Parsing Tests ====================

void test_parse_simple_equation() {
    TEST("Parse simple equation: x = 5");
    Parser p("x = 5");
    auto   eq = p.parse_equation();
    ASSERT_STR_EQ(eq->to_string(), "x = 5");
    PASS();
}

void test_parse_equation_with_operations() {
    TEST("Parse equation with operations: x + 5 = 10");
    Parser p("x + 5 = 10");
    auto   eq = p.parse_equation();
    ASSERT_STR_EQ(eq->to_string(), "(x + 5) = 10");
    PASS();
}

void test_parse_complex_equation() {
    TEST("Parse complex equation: 2 \\times x - 3 = 7");
    Parser p("2 \\times x - 3 = 7");
    auto   eq = p.parse_equation();
    ASSERT_STR_EQ(eq->to_string(), "((2 * x) - 3) = 7");
    PASS();
}

void test_parse_equation_both_sides() {
    TEST("Parse equation with expressions on both sides: x + 2 = y - 3");
    Parser p("x + 2 = y - 3");
    auto   eq = p.parse_equation();
    ASSERT_STR_EQ(eq->to_string(), "(x + 2) = (y - 3)");
    PASS();
}

// ==================== Error Handling Tests ====================

void test_parse_empty_string() {
    TEST("Parse empty string error");
    try {
        Parser p("");
        auto   expr = p.parse();
        std::cerr << "  FAILED: Should have thrown ParseError" << std::endl;
        throw std::runtime_error("Unexpected character");
    } catch (const ParseError &) {
        PASS();
    }
}

void test_parse_missing_closing_paren() {
    TEST("Parse missing closing parenthesis error: (2 + 3");
    try {
        Parser p("(2 + 3");
        auto   expr = p.parse();
        std::cerr << "  FAILED: Should have thrown ParseError" << std::endl;
        throw std::runtime_error("Expected exception not thrown");
    } catch (const ParseError &) {
        PASS();
    }
}

void test_parse_missing_opening_paren() {
    TEST("Parse extra closing parenthesis error: 2 + 3)");
    try {
        Parser p("2 + 3)");
        auto   expr = p.parse();
        std::cerr << "  FAILED: Should have thrown ParseError" << std::endl;
        throw std::runtime_error("Expected exception not thrown");
    } catch (const ParseError &) {
        PASS();
    }
}

void test_parse_missing_operand() {
    TEST("Parse missing operand error: 2 + ");
    try {
        Parser p("2 + ");
        auto   expr = p.parse();
        std::cerr << "  FAILED: Should have thrown ParseError" << std::endl;
        throw std::runtime_error("Expected exception not thrown");
    } catch (const ParseError &) {
        PASS();
    }
}

void test_parse_sqrt_missing_brace() {
    TEST("Parse sqrt missing opening brace error: \\sqrt 16}");
    try {
        Parser p("\\sqrt 16}");
        auto   expr = p.parse();
        std::cerr << "  FAILED: Should have thrown ParseError" << std::endl;
        throw std::runtime_error("Expected exception not thrown");
    } catch (const ParseError &) {
        PASS();
    }
}

void test_parse_sqrt_missing_closing_brace() {
    TEST("Parse sqrt missing closing brace error: \\sqrt{16");
    try {
        Parser p("\\sqrt{16");
        auto   expr = p.parse();
        std::cerr << "  FAILED: Should have thrown ParseError" << std::endl;
        throw std::runtime_error("Expected exception not thrown");
    } catch (const ParseError &) {
        PASS();
    }
}

void test_parse_equation_missing_equals() {
    TEST("Parse equation missing equals sign error: x + 5");
    try {
        Parser p("x + 5");
        auto   eq = p.parse_equation();
        std::cerr << "  FAILED: Should have thrown ParseError" << std::endl;
        throw std::runtime_error("Expected exception not thrown");
    } catch (const ParseError &) {
        PASS();
    }
}

void test_parse_equation_double_equals() {
    TEST("Parse equation with double equals error: x = 5 = 10");
    try {
        Parser p("x = 5 = 10");
        auto   eq = p.parse_equation();
        std::cerr << "  FAILED: Should have thrown ParseError" << std::endl;
        throw std::runtime_error("Expected exception not thrown");
    } catch (const ParseError &) {
        PASS();
    }
}

void test_parse_trailing_characters() {
    TEST("Parse trailing characters error: 2 + 3 abc");
    try {
        Parser p("2 + 3 abc");
        auto   expr = p.parse();
        std::cerr << "  FAILED: Should have thrown ParseError" << std::endl;
        throw std::runtime_error("Expected exception not thrown");
    } catch (const ParseError &) {
        PASS();
    }
}

// ==================== Main Test Runner ====================

int main() {
    std::cout << "=== Parser Tests ===" << std::endl << std::endl;

    try {
        std::cout << "--- Number Parsing ---" << std::endl;
        test_parse_integer();
        test_parse_float();
        test_parse_zero();

        std::cout << "\n--- Variable Parsing ---" << std::endl;
        test_parse_single_variable();
        test_parse_multi_char_variable();

        std::cout << "\n--- Binary Operation Parsing ---" << std::endl;
        test_parse_addition();
        test_parse_subtraction();
        test_parse_multiplication();
        test_parse_division();
        test_parse_latex_times();
        test_parse_latex_div();

        std::cout << "\n--- Precedence Tests ---" << std::endl;
        test_parse_precedence_mul_add();
        test_parse_precedence_add_mul();
        test_parse_associativity_left();
        test_parse_complex_precedence();

        std::cout << "\n--- Parentheses Tests ---" << std::endl;
        test_parse_simple_parentheses();
        test_parse_parentheses_precedence();
        test_parse_nested_parentheses();
        test_parse_multiple_parentheses();

        std::cout << "\n--- Unary Minus Tests ---" << std::endl;
        test_parse_unary_minus_number();
        test_parse_unary_minus_variable();
        test_parse_unary_minus_expression();
        test_parse_unary_minus_in_expression();

        std::cout << "\n--- Function Parsing Tests ---" << std::endl;
        test_parse_sqrt_number();
        test_parse_sqrt_variable();
        test_parse_sqrt_expression();
        test_parse_sqrt_in_expression();
        test_parse_sqrt_multiplication();

        std::cout << "\n--- Whitespace Tests ---" << std::endl;
        test_parse_no_whitespace();
        test_parse_extra_whitespace();

        std::cout << "\n--- Equation Parsing Tests ---" << std::endl;
        test_parse_simple_equation();
        test_parse_equation_with_operations();
        test_parse_complex_equation();
        test_parse_equation_both_sides();

        std::cout << "\n--- Error Handling Tests ---" << std::endl;
        test_parse_empty_string();
        test_parse_missing_closing_paren();
        test_parse_missing_opening_paren();
        test_parse_missing_operand();
        test_parse_sqrt_missing_brace();
        test_parse_sqrt_missing_closing_brace();
        test_parse_equation_missing_equals();
        test_parse_equation_double_equals();
        test_parse_trailing_characters();

        std::cout << "\n==================================" << std::endl;
        std::cout << "All " << passed_count << "/" << test_count
                  << " tests PASSED!" << std::endl;
        return 0;

    } catch (const std::exception &e) {
        std::cerr << "\nTest suite FAILED: " << e.what() << std::endl;
        std::cerr << "Passed: " << passed_count << "/" << test_count
                  << std::endl;
        return 1;
    }
}
