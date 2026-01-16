use crate::ast::expr::Expr;

#[derive(Clone)]
pub struct Symbol {
    name: String,
}

impl Expr for Symbol {
    fn accept(&self, visitor: &mut dyn crate::ast::visitor::ExprVisitor) {
        visitor.visit_symbol(self);
    }

    fn to_string(&self) -> String {
        self.name.clone()
    }

    fn clone_box(&self) -> Box<dyn Expr> {
        Box::new(self.clone())
    }
}
