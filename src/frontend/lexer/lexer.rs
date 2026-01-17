use super::commands::COMMAND_OPERATORS;
use super::token::{Token, TokenType};
use crate::common::error::{Error, ErrorKind};
use crate::common::span::Span;

pub struct Lexer<'a> {
    input: &'a str,
    chars: Vec<char>,
    pos: usize,      // index in chars
    byte_pos: usize, // byte offset
    line: usize,
    column: usize,
}

impl<'a> Lexer<'a> {
    pub fn new(input: &'a str) -> Self {
        Self {
            input,
            chars: input.chars().collect(),
            pos: 0,
            byte_pos: 0,
            line: 1,
            column: 1,
        }
    }

    pub fn source(&self) -> &str {
        self.input
    }

    /// Tokenize the entire input and return all tokens
    pub fn lex(&mut self) -> Result<Vec<Token>, Error> {
        let mut tokens = Vec::new();
        loop {
            let token = self.next_token()?;
            let is_end = matches!(token.token_type, TokenType::End);
            tokens.push(token);
            if is_end {
                break;
            }
        }
        Ok(tokens)
    }

    fn current(&self) -> char {
        self.chars.get(self.pos).copied().unwrap_or('\0')
    }

    fn advance(&mut self) {
        if let Some(c) = self.chars.get(self.pos) {
            if *c == '\n' {
                self.line += 1;
                self.column = 1;
            } else {
                self.column += 1;
            }

            self.byte_pos += c.len_utf8();
        }

        self.pos += 1;
    }

    fn skip_whitespace(&mut self) {
        while self.current().is_whitespace() {
            self.advance();
        }
    }

    fn lex_number(&mut self) -> Result<Token, Error> {
        let start = self.byte_pos;
        let mut num_str = String::new();
        let mut has_dot = false;

        while self.current().is_ascii_digit() || (self.current() == '.' && !has_dot) {
            if self.current() == '.' {
                has_dot = true;
            }
            num_str.push(self.current());
            self.advance();
        }

        let end = self.byte_pos;

        if num_str == "." {
            return Err(Error {
                kind: ErrorKind::InvalidNumber,
                span: Span::new(start, end),
            });
        }

        let value = num_str.parse().map_err(|_| Error {
            kind: ErrorKind::InvalidNumber,
            span: Span::new(start, end),
        })?;

        Ok(Token {
            token_type: TokenType::Number { value },
            span: Span::new(start, end),
        })
    }

    fn lex_command(&mut self) -> Result<Token, Error> {
        let start = self.byte_pos;
        self.advance();

        let mut name = String::new();
        while self.current().is_alphabetic() || self.current() == '_' {
            name.push(self.current());
            self.advance();
        }

        let end = self.byte_pos;

        if let Some((_, token_type)) = COMMAND_OPERATORS.iter().find(|(cmd, _)| *cmd == name) {
            Ok(Token {
                token_type: token_type.clone(),
                span: Span::new(start, end),
            })
        } else {
            Err(Error {
                kind: ErrorKind::UnknownCommand(name),
                span: Span::new(start, end),
            })
        }
    }

    fn lex_identifier(&mut self) -> Token {
        let start = self.byte_pos;
        let mut id = String::new();

        while self.current().is_alphabetic() || self.current() == '_' {
            id.push(self.current());
            self.advance();
        }

        let end = self.byte_pos;

        Token {
            token_type: TokenType::Identifier { name: id },
            span: Span::new(start, end),
        }
    }

    fn lex_symbol(&mut self) -> Option<Token> {
        let start = self.byte_pos;
        let token = match self.current() {
            '+' => TokenType::Plus,
            '-' => TokenType::Minus,
            '*' => TokenType::Mul,
            '/' => TokenType::Div,
            '^' => TokenType::POW,
            '(' => TokenType::LParen,
            ')' => TokenType::RParen,
            '{' => TokenType::LBrace,
            '}' => TokenType::RBrace,
            '=' => TokenType::Equal,
            _ => return None,
        };

        self.advance();
        let end = self.byte_pos;
        Some(Token {
            token_type: token,
            span: Span::new(start, end),
        })
    }

    fn next_token(&mut self) -> Result<Token, Error> {
        self.skip_whitespace();

        let start = self.byte_pos;

        match self.current() {
            '\0' => Ok(Token {
                token_type: TokenType::End,
                span: Span::new(start, start),
            }),
            c if c.is_ascii_digit() || c == '.' => self.lex_number(),
            '\\' => self.lex_command(),
            c if c.is_alphabetic() => Ok(self.lex_identifier()),
            _ => {
                if let Some(token) = self.lex_symbol() {
                    Ok(token)
                } else {
                    let c = self.current();
                    self.advance();
                    let end = self.byte_pos;
                    Err(Error {
                        kind: ErrorKind::UnexpectedCharacter(c),
                        span: Span::new(start, end),
                    })
                }
            }
        }
    }
}
