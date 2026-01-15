#ifndef PARSER_H
#define PARSER_H

#include "../../core/ast/binary.h"
#include "../../core/ast/equation.h"
#include "../../core/ast/expr.h"
#include "../../core/ast/function.h"
#include "../../core/ast/number.h"
#include "../../core/ast/symbol.h"
#include "../../core/common/error.h"
#include "../lexer/lexer.h"
#include "../lexer/token.h"
#include <cctype>
#include <memory>
#include <string>

namespace latex_solver {

    class Parser {
        private:
        Lexer       lexer_;
        Token       current_token_;

        // helpers functions
        void        advance();

        void        expect(TokenType type, const std::string& msg);

        Token       consume(TokenType type, const std::string& msg);

        // all parsing functions
        double      parse_number();

        std::string parse_identifier();

        ExprPtr     parse_primary();

        ExprPtr     parse_unary();

        ExprPtr     parse_power();

        ExprPtr     parse_multiplicative();

        ExprPtr     parse_additive();

        ExprPtr     parse_expression();

        public:
        explicit Parser(const std::string& input);

        ExprPtr     parse();

        EquationPtr parse_equation();
    };

} // namespace latex_solver

#endif
