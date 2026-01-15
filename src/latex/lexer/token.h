#ifndef TOKEN_H
#define TOKEN_H
#include <string>
namespace latex_solver {
    enum class TokenType {
        End,

        Number,
        Identifier,

        Function,

        Plus,
        Minus,
        Mul,
        Div,
        POW,

        Equal,

        LParen,
        RParen,
        LBrace,
        RBrace,
    };

    struct Token {
        TokenType   type;
        std::string lexeme;
        double      number_value; // use only if TokenType == Number
    };

    inline const char* token_type_name(TokenType type) {
        switch (type) {
        case TokenType::End:
            return "end of input";
        case TokenType::Number:
            return "number -> numeric literal e.g., 3.14, 42";
        case TokenType::Identifier:
            return "identifier -> variable or symbol e.g., x, y, var1";
        case TokenType::Function:
            return "function -> e.g., \\sqrt, \\sin";
        case TokenType::Plus:
            return "plus -> +";
        case TokenType::Minus:
            return "minus -> -";
        case TokenType::Mul:
            return "mul -> *";
        case TokenType::Div:
            return "div -> /";
        case TokenType::POW:
            return "power -> ^";
        case TokenType::Equal:
            return "equal -> =";
        case TokenType::LParen:
            return "left parenthesis -> (";
        case TokenType::RParen:
            return "right parenthesis -> )";
        case TokenType::LBrace:
            return "left brace -> {";
        case TokenType::RBrace:
            return "right brace -> }";
        default:
            return "Unknown";
        }
    }

} // namespace latex_solver

#endif