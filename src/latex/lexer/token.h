#ifndef TOKEN_H
#define TOKEN_H
#include <string>
namespace latex_solver {

    /**
     * @enum TokenType
     * @brief Represents all lexical token kinds produced by the lexer.
     *
     * TokenType is used by the lexer to classify pieces of the input stream
     * into meaningful symbols (tokens). The parser then consumes these tokens
     * to build an abstract syntax tree (AST).
     *
     * Design notes:
     * - TokenType values are atomic and never nested.
     * - Structural meaning (precedence, grouping, nesting) is handled by the
     * parser, not by the lexer.
     * - Some token types carry additional data via the Token struct
     *   (e.g., Number has a numeric value, Identifier has a name).
     */
    enum class TokenType {

        /**
         * @brief End-of-input marker.
         *
         * Emitted when the lexer reaches the end of the input stream.
         * Used by the parser to detect completion and validate syntax.
         */
        End,

        /**
         * @brief Numeric literal.
         *
         * Represents integer or floating-point numbers.
         * The actual numeric value is stored in Token::number_value.
         *
         * Examples:
         *   42
         *   3.14
         *   .5
         */
        Number,

        /**
         * @brief Identifier (variable or symbol name).
         *
         * Represents user-defined names such as variables.
         * The identifier text is stored in Token::lexeme.
         *
         * Examples:
         *   x
         *   y1
         *   velocity
         */
        Identifier,

        /**
         * @brief Function keyword (e.g., '\\sqrt').
         *
         * Represents function names in LaTeX-style syntax.
         * The function name is stored in Token::lexeme.
         *
         * Examples:
         *   \\sqrt
         */
        Function,
        /**
         * @brief Addition operator ('+').
         */
        Plus,

        /**
         * @brief Subtraction operator ('-').
         *
         * Can represent either unary or binary minus.
         * The distinction is resolved by the parser based on context.
         */
        Minus,

        /**
         * @brief Multiplication operator ('*' or '\\times').
         */
        Mul,

        /**
         * @brief Division operator ('/' or '\\div').
         */
        Div,

        /**
         * @brief Equality operator ('=').
         *
         * Used in equations to separate left-hand and right-hand expressions.
         */
        Equal,

        /**
         * @brief Left parenthesis '('.
         *
         * Used for grouping expressions.
         */
        LParen,

        /**
         * @brief Right parenthesis ')'.
         */
        RParen,

        /**
         * @brief Left brace '{'.
         *
         * Commonly used for function arguments in LaTeX-style syntax,
         * such as \\sqrt{...}.
         */
        LBrace,

        /**
         * @brief Right brace '}'.
         */
        RBrace,
    };

    struct Token {
        TokenType   type;
        std::string lexeme;
        double      number_value; // use only if TokenType == Number
    };

} // namespace latex_solver

#endif