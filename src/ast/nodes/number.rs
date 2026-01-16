use crate::ast::expr::Expr;
use crate::common::utils::trim_trailing;

#[derive(Clone)]
pub struct Number {
    value: f64,
}

impl Expr for Number {
    fn accept(&self, visitor: &mut dyn crate::ast::visitor::ExprVisitor) {
        visitor.visit_number(self);
    }

    fn to_string(&self) -> String {
        let mut result = self.value.to_string();
        trim_trailing('0', &mut result);
        return result;
    }

    fn clone_box(&self) -> Box<dyn Expr> {
        Box::new(self.clone())
    }
}
