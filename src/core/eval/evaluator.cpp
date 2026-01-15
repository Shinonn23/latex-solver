#include "evaluator.h"

#include "../ast/binary.h"
#include "../ast/function.h"
#include "../ast/number.h"
#include "../ast/symbol.h"
#include "../common/error.h"

#include <cmath>
#include <stdexcept>

namespace latex_solver {

    // constructor
    Evaluator::Evaluator(const Context &ctx) : context_(ctx), result_(0.0) {}

    // evaluate
    double Evaluator::evaluate(const Expr &expr) {
        expr.accept(*this);
        return result_;
    }

    // Number
    void Evaluator::visit(const Number &node) { result_ = node.value(); }

    // Symbol
    void Evaluator::visit(const Symbol &node) {
        auto value = context_.get(node.name());
        if (!value.has_value()) {
            throw UndefinedVariableError(node.name());
        }
        result_ = value.value();
    }

    // BinaryOp
    void Evaluator::visit(const BinaryOp &node) {
        node.left().accept(*this);
        double left_val = result_;

        node.right().accept(*this);
        double right_val = result_;

        switch (node.op()) {
        case BinaryOpType::ADD:
            result_ = left_val + right_val;
            break;
        case BinaryOpType::SUB:
            result_ = left_val - right_val;
            break;
        case BinaryOpType::MUL:
            result_ = left_val * right_val;
            break;
        case BinaryOpType::DIV:
            if (std::abs(right_val) < 1e-10) {
                throw DivisionByZeroError();
            }
            result_ = left_val / right_val;
            break;
        case BinaryOpType::POW:
            result_ = std::pow(left_val, right_val);
            break;
        }
    }

    // Function
    void Evaluator::visit(const Function &node) {
        node.argument().accept(*this);
        double arg_val = result_;

        if (node.name() == "sqrt") {
            if (arg_val < 0) {
                throw std::runtime_error(
                    "Cannot take square root of negative number");
            }
            result_ = std::sqrt(arg_val);
        } else {
            throw std::runtime_error("Unknown function: " + node.name());
        }
    }

} // namespace latex_solver
