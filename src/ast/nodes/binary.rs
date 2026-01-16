use crate::ast::expr::Expr;
use crate::ast::visitor::ExprVisitor;

#[derive(Clone)]
pub enum OperatorType {
    ADD = 0,
    SUB = 1,
    MUL = 2,
    DIV = 3,
}

impl OperatorType {
    fn to_string(&self) -> String {
        match self {
            OperatorType::ADD => "+",
            OperatorType::SUB => "-",
            OperatorType::MUL => "*",
            OperatorType::DIV => "/",
        }
        .to_string()
    }
}

#[derive(Clone)]
pub struct BinaryOperation {
    left: Box<dyn Expr>,
    operator: OperatorType,
    right: Box<dyn Expr>,
}

impl Expr for BinaryOperation {
    fn accept(&self, visitor: &mut dyn ExprVisitor) {
        visitor.visit_binary_op(self);
    }

    fn to_string(&self) -> String {
        format!(
            "({} {} {})",
            self.left.to_string(),
            self.operator.to_string(),
            self.right.to_string()
        )
    }

    fn clone_box(&self) -> Box<dyn Expr> {
        Box::new(self.clone())
    }
}
