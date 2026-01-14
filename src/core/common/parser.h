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

class Parser {
  private:
	std::string input_; // The input string to parse (e.g., "2 + 3 * 4")
	size_t pos_;		// Current position in the input string

	char current() const {
		if (pos_ >= input_.size())
			return '\0';
		return input_[pos_];
	}

	void advance() { pos_++; }

	void skip_whitespace() {
		while (std::isspace(current())) {
			advance();
		}
	}

	double parse_number() {
		std::string num_str;
		bool has_dot = false;

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

	std::string parse_identifier() {
		std::string id;
		while (std::isalnum(current()) || current() == '_') {
			id += current();
			advance();
		}
		return id;
	}

	ExprPtr parse_primary() {
		skip_whitespace();

		if (current() == '-') {
			advance();
			auto expr = parse_primary();
			return std::make_unique<BinaryOp>(std::make_unique<Number>(0),
											  std::move(expr),
											  BinaryOpType::SUB);
		}

		if (starts_with("\\sqrt")) {
			pos_ += 5;
			skip_whitespace();
			if (current() != '{') {
				throw ParseError("Expected '{' after \\sqrt");
			}
			advance();
			auto expr = parse_expression();
			skip_whitespace();
			if (current() != '}') {
				throw ParseError("Expected '}' after \\sqrt argument");
			}
			advance();
			return std::make_unique<Function>("sqrt", std::move(expr));
		}

		if (current() == '(') {
			advance();
			auto expr = parse_expression();
			skip_whitespace();
			if (current() != ')') {
				throw ParseError("Expected ')'");
			}
			advance();
			return expr;
		}

		if (std::isdigit(current())) {
			return std::make_unique<Number>(parse_number());
		}

		if (std::isalpha(current())) {
			return std::make_unique<Symbol>(parse_identifier());
		}

		throw ParseError("Unexpected character: " + std::string(1, current()));
	}

	bool starts_with(const std::string &str) const {
		if (pos_ + str.length() > input_.size())
			return false;
		return input_.substr(pos_, str.length()) == str;
	}

	ExprPtr parse_multiplicative() {
		auto left = parse_primary();

		while (true) {
			skip_whitespace();

			BinaryOpType op_type;
			bool found_op = false;

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

	ExprPtr parse_expression() { return parse_additive(); }

  public:
	explicit Parser(const std::string &input) : input_(input), pos_(0) {}

	ExprPtr parse() {
		auto expr = parse_expression();
		skip_whitespace();
		if (current() != '\0') {
			throw ParseError("Unexpected characters after expression");
		}
		return expr;
	}

	EquationPtr parse_equation() {
		auto left = parse_expression();
		skip_whitespace();

		if (current() != '=') {
			throw ParseError("Expected '=' in equation");
		}
		advance();

		auto right = parse_expression();
		skip_whitespace();

		if (current() != '\0') {
			throw ParseError("Unexpected characters after equation");
		}

		return std::make_unique<Equation>(std::move(left), std::move(right));
	}
};

} // namespace latex_solver

#endif
