#include "../core/common/error.h"
#include "../core/lexer/lexer.h"
#include "../core/lexer/token.h"
#include <cassert>
#include <iostream>
#include <vector>


using namespace latex_solver;

int test_count   = 0;
int passed_count = 0;

#define TEST(name)                                                             \
    test_count++;                                                              \
    std::cout << "Test " << test_count << ": " << name << "..." << std::endl

#define ASSERT_EQ(actual, expected)                                            \
    if ((actual) != (expected)) {                                              \
        std::cerr << "  FAILED: Expected " << token_type_str(expected)         \
                  << " but got " << token_type_str(actual) << std::endl;       \
        throw std::runtime_error("Assertion failed");                          \
    }

#define ASSERT_STR_EQ(actual, expected)                                        \
    if ((actual) != (expected)) {                                              \
        std::cerr << "  FAILED: Expected " << (expected) << " but got "        \
                  << (actual) << std::endl;                                    \
        throw std::runtime_error("Assertion failed");                          \
    }

#define ASSERT_NEAR(actual, expected)                                          \
    if (std::abs((actual) - (expected)) > 1e-10) {                             \
        std::cerr << "  FAILED: Expected " << (expected) << " but got "        \
                  << (actual) << std::endl;                                    \
        throw std::runtime_error("Assertion failed");                          \
    }

#define ASSERT_THROW(expr, exception_type)                                     \
    try {                                                                      \
        expr;                                                                  \
        std::cerr << "  FAILED: Expected exception " #exception_type           \
                  << " but none was thrown" << std::endl;                      \
        throw std::runtime_error("Assertion failed");                          \
    } catch (const exception_type &) {                                         \
    }

#define PASS()                                                                 \
    std::cout << "  PASSED" << std::endl;                                      \
    passed_count++

// Helper to convert TokenType to string for debugging
const char *token_type_str(TokenType type) {
    switch (type) {
    case TokenType::End:
        return "End";
    case TokenType::Number:
        return "Number";
    case TokenType::Identifier:
        return "Identifier";
    case TokenType::Function:
        return "Function";
    case TokenType::Plus:
        return "Plus";
    case TokenType::Minus:
        return "Minus";
    case TokenType::Mul:
        return "Mul";
    case TokenType::Div:
        return "Div";
    case TokenType::Equal:
        return "Equal";
    case TokenType::LParen:
        return "LParen";
    case TokenType::RParen:
        return "RParen";
    case TokenType::LBrace:
        return "LBrace";
    case TokenType::RBrace:
        return "RBrace";
    default:
        return "Unknown";
    }
}

void test_single_number() {
    TEST("Single integer");
    Lexer lexer("42");
    Token tok = lexer.nextToken();
    ASSERT_EQ(tok.type, TokenType::Number);
    ASSERT_NEAR(tok.number_value, 42.0);
    PASS();
}

void test_decimal_number() {
    TEST("Decimal number");
    Lexer lexer("3.14");
    Token tok = lexer.nextToken();
    ASSERT_EQ(tok.type, TokenType::Number);
    ASSERT_NEAR(tok.number_value, 3.14);
    PASS();
}

void test_number_starting_with_dot() {
    TEST("Number starting with dot");
    Lexer lexer(".5");
    Token tok = lexer.nextToken();
    ASSERT_EQ(tok.type, TokenType::Number);
    ASSERT_NEAR(tok.number_value, 0.5);
    PASS();
}

void test_invalid_dot_only() {
    TEST("Invalid: dot only");
    Lexer lexer(".");
    ASSERT_THROW(lexer.nextToken(), LexerError);
    PASS();
}

void test_single_identifier() {
    TEST("Single character identifier");
    Lexer lexer("x");
    Token tok = lexer.nextToken();
    ASSERT_EQ(tok.type, TokenType::Identifier);
    ASSERT_STR_EQ(tok.lexeme, "x");
    PASS();
}

void test_multi_char_identifier() {
    TEST("Multi-character identifier");
    Lexer lexer("velocity");
    Token tok = lexer.nextToken();
    ASSERT_EQ(tok.type, TokenType::Identifier);
    ASSERT_STR_EQ(tok.lexeme, "velocity");
    PASS();
}

void test_identifier_with_number() {
    TEST("Identifier with numbers");
    Lexer lexer("x1");
    Token tok = lexer.nextToken();
    ASSERT_EQ(tok.type, TokenType::Identifier);
    ASSERT_STR_EQ(tok.lexeme, "x1");
    PASS();
}

void test_identifier_with_underscore() {
    TEST("Identifier with underscore");
    Lexer lexer("my_var");
    Token tok = lexer.nextToken();
    ASSERT_EQ(tok.type, TokenType::Identifier);
    ASSERT_STR_EQ(tok.lexeme, "my_var");
    PASS();
}

void test_latex_sqrt_function() {
    TEST("LaTeX sqrt function");
    Lexer lexer("\\sqrt");
    Token tok = lexer.nextToken();
    ASSERT_EQ(tok.type, TokenType::Function);
    ASSERT_STR_EQ(tok.lexeme, "sqrt");
    PASS();
}

// void test_latex_sin_function() {
//     TEST("LaTeX sin function");
//     Lexer lexer("\\sin");
//     Token tok = lexer.nextToken();
//     ASSERT_EQ(tok.type, TokenType::Function);
//     ASSERT_STR_EQ(tok.lexeme, "sin");
//     PASS();
// }

// void test_latex_cos_function() {
//     TEST("LaTeX cos function");
//     Lexer lexer("\\cos");
//     Token tok = lexer.nextToken();
//     ASSERT_EQ(tok.type, TokenType::Function);
//     ASSERT_STR_EQ(tok.lexeme, "cos");
//     PASS();
// }

void test_latex_times_operator() {
    TEST("LaTeX times operator");
    Lexer lexer("\\times");
    Token tok = lexer.nextToken();
    ASSERT_EQ(tok.type, TokenType::Mul);
    PASS();
}

void test_latex_div_operator() {
    TEST("LaTeX div operator");
    Lexer lexer("\\div");
    Token tok = lexer.nextToken();
    ASSERT_EQ(tok.type, TokenType::Div);
    PASS();
}

void test_unknown_latex_command() {
    TEST("Unknown LaTeX command");
    Lexer lexer("\\alpha");
    ASSERT_THROW(lexer.nextToken(), LexerError);
    PASS();
}

void test_plus_operator() {
    TEST("Plus operator");
    Lexer lexer("+");
    Token tok = lexer.nextToken();
    ASSERT_EQ(tok.type, TokenType::Plus);
    PASS();
}

void test_minus_operator() {
    TEST("Minus operator");
    Lexer lexer("-");
    Token tok = lexer.nextToken();
    ASSERT_EQ(tok.type, TokenType::Minus);
    PASS();
}

void test_mul_operator() {
    TEST("Multiplication operator");
    Lexer lexer("*");
    Token tok = lexer.nextToken();
    ASSERT_EQ(tok.type, TokenType::Mul);
    PASS();
}

void test_div_operator() {
    TEST("Division operator");
    Lexer lexer("/");
    Token tok = lexer.nextToken();
    ASSERT_EQ(tok.type, TokenType::Div);
    PASS();
}

void test_equal_operator() {
    TEST("Equal operator");
    Lexer lexer("=");
    Token tok = lexer.nextToken();
    ASSERT_EQ(tok.type, TokenType::Equal);
    PASS();
}

void test_parentheses() {
    TEST("Parentheses");
    Lexer lexer("()");
    Token tok1 = lexer.nextToken();
    Token tok2 = lexer.nextToken();
    ASSERT_EQ(tok1.type, TokenType::LParen);
    ASSERT_EQ(tok2.type, TokenType::RParen);
    PASS();
}

void test_braces() {
    TEST("Braces");
    Lexer lexer("{}");
    Token tok1 = lexer.nextToken();
    Token tok2 = lexer.nextToken();
    ASSERT_EQ(tok1.type, TokenType::LBrace);
    ASSERT_EQ(tok2.type, TokenType::RBrace);
    PASS();
}

void test_whitespace_handling() {
    TEST("Whitespace handling");
    Lexer lexer("  42  +  3  ");
    Token tok1 = lexer.nextToken();
    Token tok2 = lexer.nextToken();
    Token tok3 = lexer.nextToken();
    ASSERT_EQ(tok1.type, TokenType::Number);
    ASSERT_NEAR(tok1.number_value, 42.0);
    ASSERT_EQ(tok2.type, TokenType::Plus);
    ASSERT_EQ(tok3.type, TokenType::Number);
    ASSERT_NEAR(tok3.number_value, 3.0);
    PASS();
}

void test_expression_tokenization() {
    TEST("Expression tokenization: 2 + x * 3");
    Lexer                  lexer("2 + x * 3");
    std::vector<TokenType> expected = {TokenType::Number,     TokenType::Plus,
                                       TokenType::Identifier, TokenType::Mul,
                                       TokenType::Number,     TokenType::End};
    for (size_t i = 0; i < expected.size(); i++) {
        Token tok = lexer.nextToken();
        if (tok.type != expected[i]) {
            std::cerr << "  FAILED: Token " << i << " expected "
                      << token_type_str(expected[i]) << " but got "
                      << token_type_str(tok.type) << std::endl;
            throw std::runtime_error("Assertion failed");
        }
    }
    PASS();
}

void test_latex_expression() {
    TEST("LaTeX expression: \\sqrt{x} + 2");
    Lexer                  lexer("\\sqrt{x} + 2");
    std::vector<TokenType> expected = {TokenType::Function,   TokenType::LBrace,
                                       TokenType::Identifier, TokenType::RBrace,
                                       TokenType::Plus,       TokenType::Number,
                                       TokenType::End};
    for (size_t i = 0; i < expected.size(); i++) {
        Token tok = lexer.nextToken();
        if (tok.type != expected[i]) {
            std::cerr << "  FAILED: Token " << i << " expected "
                      << token_type_str(expected[i]) << " but got "
                      << token_type_str(tok.type) << std::endl;
            throw std::runtime_error("Assertion failed");
        }
    }
    PASS();
}

void test_end_token() {
    TEST("End token");
    Lexer lexer("");
    Token tok = lexer.nextToken();
    ASSERT_EQ(tok.type, TokenType::End);
    PASS();
}

void test_multiple_end_tokens() {
    TEST("Multiple end tokens");
    Lexer lexer("");
    Token tok1 = lexer.nextToken();
    Token tok2 = lexer.nextToken();
    Token tok3 = lexer.nextToken();
    ASSERT_EQ(tok1.type, TokenType::End);
    ASSERT_EQ(tok2.type, TokenType::End);
    ASSERT_EQ(tok3.type, TokenType::End);
    PASS();
}

void test_unexpected_character() {
    TEST("Unexpected character");
    Lexer lexer("@");
    ASSERT_THROW(lexer.nextToken(), LexerError);
    PASS();
}

int main() {
    std::cout << "=== Lexer Tests ===" << std::endl << std::endl;

    try {
        // Number tests
        test_single_number();
        test_decimal_number();
        test_number_starting_with_dot();
        test_invalid_dot_only();

        // Identifier tests
        test_single_identifier();
        test_multi_char_identifier();
        test_identifier_with_number();
        test_identifier_with_underscore();

        // LaTeX function tests
        test_latex_sqrt_function();
        // test_latex_sin_function();
        // test_latex_cos_function();

        // LaTeX operator tests
        test_latex_times_operator();
        test_latex_div_operator();
        test_unknown_latex_command();

        // Basic operator tests
        test_plus_operator();
        test_minus_operator();
        test_mul_operator();
        test_div_operator();
        test_equal_operator();

        // Grouping tests
        test_parentheses();
        test_braces();

        // Complex tests
        test_whitespace_handling();
        test_expression_tokenization();
        test_latex_expression();

        // End token tests
        test_end_token();
        test_multiple_end_tokens();

        // Error tests
        test_unexpected_character();

    } catch (const std::exception &e) {
        std::cerr << "Test suite terminated early: " << e.what() << std::endl;
    }

    std::cout << std::endl
              << "=== Results ===" << std::endl
              << "Passed: " << passed_count << "/" << test_count << std::endl;

    return (passed_count == test_count) ? 0 : 1;
}
