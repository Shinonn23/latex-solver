#ifndef PARSER_H
#define PARSER_H

#include "../../core/ast/binary.h"
#include "../../core/ast/equation.h"
#include "../../core/ast/expr.h"
#include "../../core/ast/function.h"
#include "../../core/ast/number.h"
#include "../../core/ast/symbol.h"
#include "../../core/common/error.h"
#include <cctype>
#include <memory>
#include <string>

namespace latex_solver {
/**
 * Parser class for parsing mathematical expressions.
 *
 * Example inputs:
 * - "42" -> Number(42)
 * - "x" -> Symbol("x")
 * - "2 + 3" -> BinaryOp(ADD, Number(2), Number(3))
 * - "x * y + 5" -> BinaryOp(ADD, BinaryOp(MUL, Symbol("x"), Symbol("y")),
 * Number(5))
 * - "(2 + 3) * 4" -> BinaryOp(MUL, BinaryOp(ADD, Number(2), Number(3)),
 * Number(4))
 */
class Parser {
  private:
	std::string input_; // The input string to parse (e.g., "2 + 3 * 4")
	size_t pos_;		// Current position in the input string

	/**
	 * Returns the character at the current position.
	 * Returns '\0' if we've reached the end of input.
	 *
	 * Example: if input_ = "2+3" and pos_ = 1, returns '+'
	 */
	char current() const {
		if (pos_ >= input_.size())
			return '\0';
		return input_[pos_];
	}

	/**
	 * Advances the position pointer by one character.
	 * Moves from current character to the next one.
	 */
	void advance() { pos_++; }

	/**
	 * Skips all whitespace characters from the current position.
	 * Continues advancing until a non-whitespace character is found.
	 *
	 * Example: if input_ = "2  +  3" and pos_ = 1, advances to pos_ = 4 (the
	 * '+')
	 */
	void skip_whitespace() {
		while (std::isspace(current())) {
			advance();
		}
	}

	/**
	 * Parses a number from the current position.
	 * Supports both integers and floating-point numbers.
	 *
	 * Example: "42.5abc" will parse "42.5" and leave pos_ at 'a'
	 *
	 * @return The parsed number as a double
	 * @throws ParseError if no valid number is found
	 */
	double parse_number() {
		std::string num_str;
		bool has_dot = false;

		// Collect all digits and at most one decimal point
		while (std::isdigit(current()) || (current() == '.' && !has_dot)) {
			if (current() == '.')
				has_dot = true;
			num_str += current();
			advance();
		}

		if (num_str.empty()) {
			throw ParseError("Expected number");
		}

		return std::stod(num_str);
	}

	/**
	 * Parses an identifier (variable name) from the current position.
	 * Identifiers can contain letters, digits, and underscores.
	 * Must start with a letter (enforced by caller).
	 *
	 * Example: "var_name123" will parse the entire string as an identifier
	 *
	 * @return The parsed identifier as a string
	 */
	std::string parse_identifier() {
		std::string id;
		while (std::isalnum(current()) || current() == '_') {
			id += current();
			advance();
		}
		return id;
	}

	/**
	 * Parses a primary expression (highest precedence).
	 * Primary expressions include:
	 * - Numbers: "42", "3.14"
	 * - Variables: "x", "var_name"
	 * - Parenthesized expressions: "(2 + 3)"
	 * - Unary minus: "-5", "-(x + 2)"
	 *
	 * Example: "(x + 5)" will recursively parse the expression inside
	 * parentheses
	 *
	 * @return A unique pointer to the parsed expression
	 * @throws ParseError if the expression is invalid
	 */
	ExprPtr parse_primary() {
		skip_whitespace();

		// Handle unary minus
		if (current() == '-') {
			advance(); // consume '-'
			auto expr = parse_primary();
			// Create: 0 - expr or -1 * expr
			return std::make_unique<BinaryOp>(std::make_unique<Number>(0),
											  std::move(expr),
											  BinaryOpType::SUB);
		}

		// Handle \sqrt{...}
		if (starts_with("\\sqrt")) {
			pos_ += 5; // consume "\sqrt"
			skip_whitespace();
			if (current() != '{') {
				throw ParseError("Expected '{' after \\sqrt");
			}
			advance(); // consume '{'
			auto expr = parse_expression();
			skip_whitespace();
			if (current() != '}') {
				throw ParseError("Expected '}' after \\sqrt argument");
			}
			advance(); // consume '}'
			return std::make_unique<Function>("sqrt", std::move(expr));
		}

		// Handle parenthesized expressions
		if (current() == '(') {
			advance(); // consume '('
			auto expr = parse_expression();
			skip_whitespace();
			if (current() != ')') {
				throw ParseError("Expected ')'");
			}
			advance(); // consume ')'
			return expr;
		}

		// Handle numbers
		if (std::isdigit(current())) {
			return std::make_unique<Number>(parse_number());
		}

		// Handle identifiers (variables)
		if (std::isalpha(current())) {
			return std::make_unique<Symbol>(parse_identifier());
		}

		throw ParseError("Unexpected character: " + std::string(1, current()));
	}

	/**
	 * Checks if the input starts with a specific string at current position.
	 */
	bool starts_with(const std::string &str) const {
		if (pos_ + str.length() > input_.size())
			return false;
		return input_.substr(pos_, str.length()) == str;
	}

	/**
	 * Parses multiplicative expressions (*, /, \times, \div).
	 * Left-associative: "a * b * c" is parsed as "(a * b) * c"
	 * Higher precedence than addition/subtraction.
	 *
	 * Example: "2 * 3 / 4" -> BinaryOp(DIV, BinaryOp(MUL, Number(2),
	 * Number(3)), Number(4))
	 *
	 * @return A unique pointer to the parsed expression
	 */
	ExprPtr parse_multiplicative() {
		auto left = parse_primary();

		while (true) {
			skip_whitespace();

			BinaryOpType op_type;
			bool found_op = false;

			// Check for LaTeX operators first
			if (starts_with("\\times")) {
				pos_ += 6; // length of "\times"
				op_type = BinaryOpType::MUL;
				found_op = true;
			} else if (starts_with("\\div")) {
				pos_ += 4; // length of "\div"
				op_type = BinaryOpType::DIV;
				found_op = true;
			} else if (current() == '*') {
				advance();
				op_type = BinaryOpType::MUL;
				found_op = true;
			} else if (current() == '/') {
				advance();
				op_type = BinaryOpType::DIV;
				found_op = true;
			}

			if (found_op) {
				auto right = parse_primary();
				left = std::make_unique<BinaryOp>(std::move(left),
												  std::move(right), op_type);
			} else {
				break;
			}
		}

		return left;
	}

	/**
	 * Parses additive expressions (+, -).
	 * Left-associative: "a + b - c" is parsed as "(a + b) - c"
	 * Lower precedence than multiplication/division.
	 *
	 * Example: "2 + 3 * 4" -> BinaryOp(
	 *                             Number(2),
	 *                             BinaryOp(
	 *                                 Number(3),
	 *                                 Number(4),
	 *                                 MUL
	 *                             ),
	 *                             ADD
	 *                         )
	 *
	 * @return A unique pointer to the parsed expression
	 */
	ExprPtr parse_additive() {
		auto left = parse_multiplicative();

		while (true) {
			skip_whitespace();
			char op = current();

			if (op == '+' || op == '-') {
				advance();
				auto right = parse_multiplicative();
				BinaryOpType op_type =
					(op == '+') ? BinaryOpType::ADD : BinaryOpType::SUB;
				left = std::make_unique<BinaryOp>(std::move(left),
												  std::move(right), op_type);
			} else {
				break;
			}
		}

		return left;
	}

	/**
	 * Parses a complete expression.
	 * Entry point for the recursive descent parser.
	 *
	 * @return A unique pointer to the parsed expression
	 */
	ExprPtr parse_expression() { return parse_additive(); }

  public:
	/**
	 * Constructs a Parser with the given input string.
	 *
	 * @param input The mathematical expression to parse
	 */
	explicit Parser(const std::string &input) : input_(input), pos_(0) {}

	/**
	 * Parses the input string into an expression tree.
	 * Ensures the entire input is consumed.
	 *
	 * Example usage:
	 *   Parser p("2 + 3 * 4");
	 *   ExprPtr expr = p.parse();
	 *
	 * @return A unique pointer to the root of the expression tree
	 * @throws ParseError if the input is invalid or contains trailing
	 * characters
	 */
	ExprPtr parse() {
		auto expr = parse_expression();
		skip_whitespace();
		if (current() != '\0') {
			throw ParseError("Unexpected characters after expression");
		}
		return expr;
	}

	/**
	 * Parses the input string into an equation.
	 * Expects format: <expression> = <expression>
	 *
	 * Example usage:
	 *   Parser p("x + 5 = 10");
	 *   EquationPtr eq = p.parse_equation();
	 *
	 * @return A unique pointer to the parsed equation
	 * @throws ParseError if the input is invalid or doesn't contain '='
	 */
	EquationPtr parse_equation() {
		auto left = parse_expression();
		skip_whitespace();

		if (current() != '=') {
			throw ParseError("Expected '=' in equation");
		}
		advance(); // consume '='

		auto right = parse_expression();
		skip_whitespace();

		if (current() != '\0') {
			throw ParseError("Unexpected characters after equation");
		}

		return std::make_unique<Equation>(std::move(left), std::move(right));
	}
};

} // namespace latex_solver

#endif // PARSER_H
