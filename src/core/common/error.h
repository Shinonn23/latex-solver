#ifndef ERROR_H
#define ERROR_H

#include "../lexer/token.h"
#include <stdexcept>
#include <string>

namespace latex_solver {

    class EvaluationError : public std::runtime_error {
        public:
        explicit EvaluationError(const std::string &message)
            : std::runtime_error(message) {}
    };

    class DivisionByZeroError : public EvaluationError {
        public:
        DivisionByZeroError() : EvaluationError("Division by zero") {}
    };

    class UndefinedVariableError : public EvaluationError {
        public:
        explicit UndefinedVariableError(const std::string &var_name)
            : EvaluationError("Undefined variable: " + var_name) {}
    };

    class ParseError : public std::runtime_error {
        public:
        explicit ParseError(const std::string &message)
            : std::runtime_error("Parse error: " + message) {}
    };

    class UnexpectedTokenError : public ParseError {
        public:
        UnexpectedTokenError(TokenType type, const std::string &lexeme)
            : ParseError(build_message(type, lexeme)) {}

        private:
        static std::string build_message(TokenType          type,
                                         const std::string &lexeme) {
            std::string msg =
                std::string("Unexpected token: ") + token_type_name(type);
            if (!lexeme.empty()) {
                msg += " (" + lexeme + ")";
            }
            return msg;
        }
    };

    class LexerError : public std::runtime_error {
        public:
        explicit LexerError(const std::string &message)
            : std::runtime_error("Lexer error: " + message) {}
    };

    class SolverError : public std::runtime_error {
        public:
        explicit SolverError(const std::string &message)
            : std::runtime_error("Solver error: " + message) {}
    };

} // namespace latex_solver

#endif
