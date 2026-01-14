#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "../ast/binary.h"
#include "../ast/expr.h"
#include "../ast/function.h"
#include "../ast/number.h"
#include "../ast/symbol.h"
#include "context.h"
#include "error.h"
#include <cmath>

namespace latex_solver {

    class Evaluator : public ExprVisitor {
        private:
        const Context &context_;
        double         result_;

        public:
        explicit Evaluator(const Context &ctx) : context_(ctx), result_(0.0) {}

        double evaluate(const Expr &expr) {
            expr.accept(*this);
            return result_;
        }

        void visit(const Number &node) override { result_ = node.value(); }

        void visit(const Symbol &node) override {
            auto value = context_.get(node.name());
            if (!value.has_value()) {
                throw UndefinedVariableError(node.name());
            }
            result_ = value.value();
        }

        void visit(const BinaryOp &node) override {
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
            }
        }

        void visit(const Function &node) override {
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
    };

} // namespace latex_solver

#endif
