#ifndef EXPR_H
#define EXPR_H

#include <memory>
#include <string>

namespace latex_solver {

    // Forward declarations
    class Number;
    class Symbol;
    class BinaryOp;
    class Function;

    // Visitor pattern for expression traversal
    class ExprVisitor {
        public:
        virtual ~ExprVisitor()                   = default;
        virtual void visit(const Number &node)   = 0;
        virtual void visit(const Symbol &node)   = 0;
        virtual void visit(const BinaryOp &node) = 0;
        virtual void visit(const Function &node) = 0;
    };

    class Expr {
        public:
        virtual ~Expr() = default;

        // Accept visitor for traversal
        virtual void                  accept(ExprVisitor &visitor) const = 0;

        // String representation
        virtual std::string           to_string() const                  = 0;

        // Clone for copying
        virtual std::unique_ptr<Expr> clone() const                      = 0;
    };

    using ExprPtr = std::unique_ptr<Expr>;

} // namespace latex_solver

#endif
