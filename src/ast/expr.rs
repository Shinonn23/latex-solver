use super::visitor::ExprVisitor;

pub trait Expr {
    fn accept(&self, visitor: &mut dyn ExprVisitor);
    fn to_string(&self) -> String;
    fn clone_box(&self) -> Box<dyn Expr>;
}

impl Clone for Box<dyn Expr> {
    fn clone(&self) -> Self {
        self.clone_box()
    }
}

pub type ExprPtr = Box<dyn Expr>;