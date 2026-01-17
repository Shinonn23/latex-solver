use crate::common::span::Span;
use std::fmt;

#[derive(Clone, Debug)]
pub enum TokenType {
    End,

    Number { value: f64 },
    Identifier { name: String },

    Command { name: String },

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
}

impl fmt::Display for TokenType {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        let s = match self {
            TokenType::End => "end of input",
            TokenType::Number { .. } => "number (numeric literal, e.g. 3.14, 42)",
            TokenType::Identifier { .. } => "identifier (variable or symbol)",
            TokenType::Command { .. } => "Command (e.g. \\sqrt, \\sin)",
            TokenType::Plus => "+",
            TokenType::Minus => "-",
            TokenType::Mul => "*",
            TokenType::Div => "/",
            TokenType::POW => "^",
            TokenType::Equal => "=",
            TokenType::LParen => "(",
            TokenType::RParen => ")",
            TokenType::LBrace => "{",
            TokenType::RBrace => "}",
        };
        write!(f, "{s}")
    }
}

#[derive(Debug)]
pub struct Token {
    pub token_type: TokenType,
    pub span: Span,
}
