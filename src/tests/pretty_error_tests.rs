#[cfg(test)]
mod pretty_error_tests {
    use crate::common::pretty::render_error;
    use crate::frontend::lexer::lexer::Lexer;

    #[test]
    fn test_unexpected_character_error() {
        let source = "x + @";
        let mut lexer = Lexer::new(source);
        
        let error = lexer.lex().unwrap_err();
        let output = render_error(lexer.source(), &error);
        
        assert!(output.contains("unexpected character '@'"));
        assert!(output.contains("1:5"));
        assert!(output.contains("x + @"));
        assert!(output.contains("^"));
    }

    #[test]
    fn test_unknown_command_error() {
        let source = "\\unknown + 5";
        let mut lexer = Lexer::new(source);
        
        let error = lexer.lex().unwrap_err();
        let output = render_error(source, &error);
        
        assert!(output.contains("unknown command \\unknown"));
        assert!(output.contains("1:1"));
        assert!(output.contains("\\unknown + 5"));
    }

    #[test]
    fn test_invalid_number_error() {
        let source = "x + . + y";
        let mut lexer = Lexer::new(source);
        
        let error = lexer.lex().unwrap_err();
        let output = render_error(source, &error);
        
        assert!(output.contains("invalid number"));
        assert!(output.contains("1:5"));
        assert!(output.contains("^"));
    }

    #[test]
    fn test_successful_lexing() {
        let source = "x + 5 * y";
        let mut lexer = Lexer::new(source);
        
        let tokens = lexer.lex().unwrap();
        assert_eq!(tokens.len(), 6); // x, +, 5, *, y, END
    }

    #[test]
    fn test_error_span_width() {
        let source = "\\unknown";
        let mut lexer = Lexer::new(source);
        
        let error = lexer.lex().unwrap_err();
        let output = render_error(source, &error);
        
        // Should have multiple carets for multi-char error
        assert!(output.contains("^^^^^^^^"));
    }
}