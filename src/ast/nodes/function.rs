use crate::ast::expr::{Expr, ExprPtr};

#[derive(Clone)]
pub struct Function {
    name: String,
    argument: ExprPtr,
}

impl Expr for Function {
    fn accept(&self, visitor: &mut dyn crate::ast::visitor::ExprVisitor) {
        visitor.visit_function(self);
    }

    fn to_string(&self) -> String {
        format!("{}({})", self.name, self.argument.to_string())
    }

    fn clone_box(&self) -> Box<dyn Expr> {
        Box::new(self.clone())
    }
}
