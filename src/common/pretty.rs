use crate::common::error::{Error, ErrorKind};

/// Calculate line and column from byte position in source
pub fn line_col_at(source: &str, byte_pos: usize) -> (usize, usize) {
    let mut line = 1;
    let mut col = 1;

    for (i, c) in source.char_indices() {
        if i >= byte_pos {
            break;
        }

        if c == '\n' {
            line += 1;
            col = 1;
        } else {
            col += 1;
        }
    }

    (line, col)
}

/// Format error kind as a human-readable string
fn format_error_kind(kind: &ErrorKind) -> String {
    match kind {
        ErrorKind::UnexpectedCharacter(c) => format!("unexpected character '{}'", c),
        ErrorKind::InvalidNumber => "invalid number literal".to_string(),
        ErrorKind::UnknownCommand(cmd) => format!("unknown command \\{}", cmd),
        ErrorKind::UnexpectedToken { expected, found } => {
            format!("expected {}, found {}", expected, found)
        }
    }
}

/// Render a pretty error message with source context
pub fn render_error(source: &str, error: &Error) -> String {
    let (line, col) = line_col_at(source, error.span.start);

    let line_text = source.lines().nth(line - 1).unwrap_or("");

    let mut out = String::new();

    out.push_str("error: ");
    out.push_str(&format_error_kind(&error.kind));
    out.push('\n');

    out.push_str(&format!(" --> {}:{}\n", line, col));
    out.push_str("  |\n");

    out.push_str(&format!("{:2} | {}\n", line, line_text));
    out.push_str("  | ");

    // Add spaces to align with the error position
    for _ in 1..col {
        out.push(' ');
    }

    // Add carets to highlight the error span
    let width = (error.span.end - error.span.start).max(1);
    for _ in 0..width {
        out.push('^');
    }

    out
}
