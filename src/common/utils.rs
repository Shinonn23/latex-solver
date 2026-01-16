pub fn trim_trailing(end: char, s: &mut String) {
    while s.ends_with(end) {
        s.pop();
    }
    if s.ends_with('.') {
        s.pop();
    }
}
