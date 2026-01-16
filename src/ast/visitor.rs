use crate::ast::nodes::binary::BinaryOperation;
use crate::ast::nodes::function::Function;
use crate::ast::nodes::number::Number;
use crate::ast::nodes::symbol::Symbol;

pub trait ExprVisitor {
    fn visit_binary_op(&mut self, node: &BinaryOperation);
    fn visit_function(&mut self, node: &Function);
    fn visit_number(&mut self, node: &Number);
    fn visit_symbol(&mut self, node: &Symbol);
}
