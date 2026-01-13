#ifndef BINARY_H
#define BINARY_H

#include "expr.h"
#include <memory>
#include <string>
#include <stdexcept>

namespace latex_solver
{

    enum class BinaryOpType
    {
        ADD, // +
        SUB, // -
        MUL, // *
        DIV  // /
    };

    class BinaryOp : public Expr
    {
    private:
        ExprPtr left_;
        ExprPtr right_;
        BinaryOpType op_;

    public:
        BinaryOp(ExprPtr left, ExprPtr right, BinaryOpType op)
            : left_(std::move(left)), right_(std::move(right)), op_(op)
        {
            if (!left_ || !right_)
            {
                throw std::invalid_argument("BinaryOp: operands cannot be null");
            }
        }

        const Expr &left() const { return *left_; }
        const Expr &right() const { return *right_; }
        BinaryOpType op() const { return op_; }

        void accept(ExprVisitor &visitor) const override
        {
            visitor.visit(*this);
        }

        std::string to_string() const override
        {
            std::string op_str;
            switch (op_)
            {
            case BinaryOpType::ADD:
                op_str = "+";
                break;
            case BinaryOpType::SUB:
                op_str = "-";
                break;
            case BinaryOpType::MUL:
                op_str = "*";
                break;
            case BinaryOpType::DIV:
                op_str = "/";
                break;
            }
            return "(" + left_->to_string() + " " + op_str + " " + right_->to_string() + ")";
        }

        std::unique_ptr<Expr> clone() const override
        {
            return std::make_unique<BinaryOp>(
                left_->clone(),
                right_->clone(),
                op_);
        }

        static std::string op_to_string(BinaryOpType op)
        {
            switch (op)
            {
            case BinaryOpType::ADD:
                return "+";
            case BinaryOpType::SUB:
                return "-";
            case BinaryOpType::MUL:
                return "*";
            case BinaryOpType::DIV:
                return "/";
            default:
                return "?";
            }
        }
    };

} // namespace latex_solver

#endif // BINARY_H
