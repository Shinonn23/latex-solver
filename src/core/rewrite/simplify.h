#ifndef SIMPLIFY_H
#define SIMPLIFY_H

#include "../ast/expr.h"
#include "../ast/number.h"
#include "../ast/symbol.h"
#include "../ast/binary.h"
#include <memory>
#include <cmath>

namespace latex_solver
{

    class Simplifier
    {
    public:
        static ExprPtr simplify(const Expr &expr)
        {
            return simplify_expr(expr);
        }

    private:
        static ExprPtr simplify_expr(const Expr &expr)
        {
            // Try to cast to different node types
            if (const Number *num = dynamic_cast<const Number *>(&expr))
            {
                return num->clone();
            }

            if (const Symbol *sym = dynamic_cast<const Symbol *>(&expr))
            {
                return sym->clone();
            }

            if (const BinaryOp *bin = dynamic_cast<const BinaryOp *>(&expr))
            {
                return simplify_binary(*bin);
            }

            return expr.clone();
        }

        static ExprPtr simplify_binary(const BinaryOp &bin)
        {
            // Recursively simplify operands
            auto left = simplify_expr(bin.left());
            auto right = simplify_expr(bin.right());

            // Check if both operands are numbers
            const Number *left_num = dynamic_cast<const Number *>(left.get());
            const Number *right_num = dynamic_cast<const Number *>(right.get());

            if (left_num && right_num)
            {
                // Constant folding
                double result = 0.0;
                double lv = left_num->value();
                double rv = right_num->value();

                switch (bin.op())
                {
                case BinaryOpType::ADD:
                    result = lv + rv;
                    break;
                case BinaryOpType::SUB:
                    result = lv - rv;
                    break;
                case BinaryOpType::MUL:
                    result = lv * rv;
                    break;
                case BinaryOpType::DIV:
                    if (std::abs(rv) < 1e-10)
                    {
                        // Keep division by zero as is
                        return std::make_unique<BinaryOp>(std::move(left), std::move(right), bin.op());
                    }
                    result = lv / rv;
                    break;
                }
                return std::make_unique<Number>(result);
            }

            // Algebraic simplifications
            switch (bin.op())
            {
            case BinaryOpType::ADD:
                // x + 0 = x
                if (right_num && std::abs(right_num->value()) < 1e-10)
                {
                    return left;
                }
                // 0 + x = x
                if (left_num && std::abs(left_num->value()) < 1e-10)
                {
                    return right;
                }
                break;

            case BinaryOpType::SUB:
                // x - 0 = x
                if (right_num && std::abs(right_num->value()) < 1e-10)
                {
                    return left;
                }
                break;

            case BinaryOpType::MUL:
                // x * 0 = 0
                if (right_num && std::abs(right_num->value()) < 1e-10)
                {
                    return std::make_unique<Number>(0.0);
                }
                if (left_num && std::abs(left_num->value()) < 1e-10)
                {
                    return std::make_unique<Number>(0.0);
                }
                // x * 1 = x
                if (right_num && std::abs(right_num->value() - 1.0) < 1e-10)
                {
                    return left;
                }
                // 1 * x = x
                if (left_num && std::abs(left_num->value() - 1.0) < 1e-10)
                {
                    return right;
                }
                break;

            case BinaryOpType::DIV:
                // 0 / x = 0 (if x != 0)
                if (left_num && std::abs(left_num->value()) < 1e-10)
                {
                    return std::make_unique<Number>(0.0);
                }
                // x / 1 = x
                if (right_num && std::abs(right_num->value() - 1.0) < 1e-10)
                {
                    return left;
                }
                break;
            }

            // No simplification possible, return new binary op with simplified operands
            return std::make_unique<BinaryOp>(std::move(left), std::move(right), bin.op());
        }
    };

} // namespace latex_solver

#endif // SIMPLIFY_H
