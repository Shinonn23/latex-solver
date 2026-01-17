use super::token::TokenType;

pub const COMMAND_OPERATORS: &[(&str, TokenType)] = &[
    ("times", TokenType::Mul),
    ("div", TokenType::Div),
];