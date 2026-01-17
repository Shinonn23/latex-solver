use crate::common::span::Span;
use std::fmt;

#[derive(Debug, Clone)]
pub enum ErrorKind {
    UnexpectedCharacter(char),
    InvalidNumber,
    UnknownCommand(String),
    UnexpectedToken {
        expected: String,
        found: String,
    },
}

#[derive(Debug, Clone)]
pub struct Error {
    pub kind: ErrorKind,
    pub span: Span,
}

impl fmt::Display for Error {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match &self.kind {
            ErrorKind::UnexpectedCharacter(c) => {
                write!(f, "unexpected character '{}' at position {}", c, self.span.start)
            }
            ErrorKind::InvalidNumber => {
                write!(f, "invalid number at position {}", self.span.start)
            }
            ErrorKind::UnknownCommand(cmd) => {
                write!(f, "unknown command \\{} at position {}", cmd, self.span.start)
            }
            ErrorKind::UnexpectedToken { expected, found } => {
                write!(
                    f,
                    "expected {}, but found {} at position {}",
                    expected, found, self.span.start
                )
            }
        }
    }
}

impl std::error::Error for Error {}
