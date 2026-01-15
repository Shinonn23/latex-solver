#ifndef LEXER_H
#define LEXER_H
#include "token.h"

namespace latex_solver {
    class Lexer {
        private:
        std::string input_;
        size_t      pos_;

        char        current() const {
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

        public:
        explicit Lexer(const std::string &input) : input_(input), pos_(0) {}

        Token nextToken() {
            skip_whitespace();

            if (current() == '\0') {
                return Token{TokenType::End, "", 0};
            }

            // Specific: numbers
            if (std::isdigit(current()) || current() == '.') {
                std::string num_str;
                bool        has_dot = false;

                while (std::isdigit(current()) ||
                       (current() == '.' && !has_dot)) {
                    if (current() == '.')
                        has_dot = true;
                    num_str += current();
                    advance();
                }
                if (num_str.empty() || num_str == ".") {
                    throw LexerError("Expected number");
                }

                return Token{TokenType::Number, "", std::stod(num_str)};
            }

            // Specific: keywords starting with backslash
            if (current() == '\\') {
                advance(); // consume the backslash
                std::string func_;
                while (std::isalnum(current()) || current() == '_') {
                    func_ += current();
                    advance();
                }

                // Operators
                if (func_ == "times") {
                    return Token{TokenType::Mul, "", 0};
                } else if (func_ == "div") {
                    return Token{TokenType::Div, "", 0};
                }

                // Functions
                if (func_ == "sqrt" || func_ == "sin" || func_ == "cos" ||
                    func_ == "tan" || func_ == "ln" || func_ == "log" ||
                    func_ == "exp" || func_ == "abs") {
                    return Token{TokenType::Function, func_, 0};
                }

                throw LexerError("Unknown LaTeX command: \\" + func_);
            }

            // Generic: identifiers
            if (std::isalpha(current())) {
                std::string id;
                while (std::isalnum(current()) || current() == '_') {
                    id += current();
                    advance();
                }
                return Token{TokenType::Identifier, id, 0};
            }

            // Fallback: operators
            switch (current()) {
            case '+':
                advance();
                return Token{TokenType::Plus, "", 0};
            case '-':
                advance();
                return Token{TokenType::Minus, "", 0};
            case '*':
                advance();
                return Token{TokenType::Mul, "", 0};
            case '/':
                advance();
                return Token{TokenType::Div, "", 0};
            case '(':
                advance();
                return Token{TokenType::LParen, "", 0};
            case ')':
                advance();
                return Token{TokenType::RParen, "", 0};
            case '{':
                advance();
                return Token{TokenType::LBrace, "", 0};
            case '}':
                advance();
                return Token{TokenType::RBrace, "", 0};
            case '=':
                advance();
                return Token{TokenType::Equal, "", 0};
            }

            throw LexerError("Unexpected character");
        }
    };

} // namespace latex_solver

#endif