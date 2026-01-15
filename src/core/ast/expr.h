#ifndef EXPR_H
#define EXPR_H

#include <memory>
#include <string>

namespace latex_solver {

    class Number;
    class Symbol;
    class BinaryOp;
    class Function;

    class ExprVisitor {
        public:
        virtual ~ExprVisitor()                 = default; // Virtual destructor
        virtual void visit(const Number &node) = 0;
        virtual void visit(const Symbol &node) = 0;
        virtual void visit(const BinaryOp &node) = 0;
        virtual void visit(const Function &node) = 0;
    };

    class Expr {
        public:
        virtual ~Expr() = default; // Virtual destructor for base class
        virtual void                  accept(ExprVisitor &visitor) const = 0;
        virtual std::string           to_string() const                  = 0;
        virtual std::unique_ptr<Expr> clone() const                      = 0;
    };

    using ExprPtr =
        std::unique_ptr<Expr>; // type alias for unique pointer to Expr

} // namespace latex_solver

#endif
