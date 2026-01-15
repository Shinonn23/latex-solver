#ifndef ERROR_H
#define ERROR_H

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
