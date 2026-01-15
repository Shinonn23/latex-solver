#ifndef PARSER_H
#define PARSER_H

#include "../../core/ast/binary.h"
#include "../../core/ast/equation.h"
#include "../../core/ast/expr.h"
#include "../../core/ast/function.h"
#include "../../core/ast/number.h"
#include "../../core/ast/symbol.h"
#include "../../core/common/error.h"
#include "../../latex/lexer/lexer.h"
#include "../../latex/lexer/token.h"
#include <cctype>
#include <memory>
#include <string>

namespace latex_solver {

    class Parser {
        private:
        Lexer lexer_;
        Token current_token_;

        // helpers functions
        void  advance() { current_token_ = lexer_.nextToken(); }

        void  expect(TokenType type, const std::string& msg) {
            if (current_token_.type != type) {
                throw ParseError(msg);
            }
            advance();
        }

        Token consume(TokenType type, const std::string& msg) {
            if (current_token_.type != type) {
                throw ParseError(msg);
            }
            Token tok = current_token_;
            advance();
            return tok;
        }

        // all parsing functions
        double parse_number() {
            auto token = consume(TokenType::Number, "Expected number");
            return token.number_value;
        }

        std::string parse_identifier() {
            auto token = consume(TokenType::Identifier, "Expected identifier");
            return token.lexeme;
        }

        ExprPtr parse_primary() {
            if (current_token_.type == TokenType::Function) {
                std::string func_name = current_token_.lexeme;
                advance();
                expect(TokenType::LBrace, "Expected '{' after function name");
                auto expr = parse_expression();
                expect(TokenType::RBrace,
                       "Expected '}' after function argument");
                return std::make_unique<Function>(func_name, std::move(expr));
            }

            if (current_token_.type == TokenType::LParen) {
                advance();
                auto expr = parse_expression();
                expect(TokenType::RParen, "Expected ')'");
                return expr;
            }

            if (current_token_.type == TokenType::LBrace) {
                advance();
                auto expr = parse_expression();
                expect(TokenType::RBrace, "Expected '}'");
                return expr;
            }

            if (current_token_.type == TokenType::Number) {
                return std::make_unique<Number>(parse_number());
            }

            if (current_token_.type == TokenType::Identifier) {
                return std::make_unique<Symbol>(parse_identifier());
            }

            throw UnexpectedTokenError(current_token_.type,
                                       current_token_.lexeme);
        }

        ExprPtr parse_unary() {
            if (current_token_.type == TokenType::Minus) {
                advance();
                auto expr = parse_unary();
                return std::make_unique<BinaryOp>(std::make_unique<Number>(0),
                                                  std::move(expr),
                                                  BinaryOpType::SUB);
            } else if (current_token_.type == TokenType::Plus) {
                advance();
                return parse_unary();
            }
            return parse_primary();
        }

        ExprPtr parse_power() {
            auto left = parse_unary();

            while (current_token_.type == TokenType::POW) {
                advance();
                auto right = parse_unary();
                left       = std::make_unique<BinaryOp>(
                    std::move(left), std::move(right), BinaryOpType::POW);
            }

            return left;
        }

        ExprPtr parse_multiplicative() {
            auto left = parse_power();

            while (true) {
                BinaryOpType op_type;
                bool         found_op = false;

                if (current_token_.type == TokenType::Mul) {
                    advance();
                    op_type  = BinaryOpType::MUL;
                    found_op = true;
                } else if (current_token_.type == TokenType::Div) {
                    advance();
                    op_type  = BinaryOpType::DIV;
                    found_op = true;
                }

                if (found_op) {
                    auto right = parse_power();
                    left       = std::make_unique<BinaryOp>(
                        std::move(left), std::move(right), op_type);
                } else {
                    break;
                }
            }

            return left;
        }

        ExprPtr parse_additive() {
            auto left = parse_multiplicative();

            while (true) {
                if (current_token_.type == TokenType::Plus ||
                    current_token_.type == TokenType::Minus) {
                    BinaryOpType op_type =
                        (current_token_.type == TokenType::Plus)
                            ? BinaryOpType::ADD
                            : BinaryOpType::SUB;
                    advance();
                    auto right = parse_multiplicative();
                    left       = std::make_unique<BinaryOp>(
                        std::move(left), std::move(right), op_type);
                } else {
                    break;
                }
            }

            return left;
        }

        ExprPtr parse_expression() { return parse_additive(); }

        public:
        explicit Parser(const std::string& input) : lexer_(input) {
            current_token_ = lexer_.nextToken();
        }

        ExprPtr parse() {
            auto expr = parse_expression();
            expect(TokenType::End, "Unexpected input after expression");
            return expr;
        }

        EquationPtr parse_equation() {
            auto left = parse_expression();
            expect(TokenType::Equal, "Expected '=' in equation");
            auto right = parse_expression();
            return std::make_unique<Equation>(std::move(left),
                                              std::move(right));
        }
    };

} // namespace latex_solver

#endif
