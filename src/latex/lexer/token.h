#ifndef TOKEN_H
#define TOKEN_H
#include <string>
namespace latex_solver {

    enum class TokenType {
        End,

        Number,
        Identifier,

        Plus,
        Minus,
        Mul,
        Div,

        Equal,

        LParen,
        RParen,
        LBrace,
        RBrace,

        Sqrt
    };

    struct Token {
        TokenType   type;
        std::string lexeme;
        double      number_value; // use only if TokenType == Number
    };

} // namespace latex_solver

#endif