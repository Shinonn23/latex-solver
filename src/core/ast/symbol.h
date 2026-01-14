#ifndef SYMBOL_H
#define SYMBOL_H

#include "expr.h"
#include <string>

namespace latex_solver
{

    class Symbol : public Expr
    {
    private:
        std::string name_;

    public:
        explicit Symbol(const std::string &name) : name_(name) {}

        const std::string &name() const { return name_; }

        void accept(ExprVisitor &visitor) const override
        {
            visitor.visit(*this);
        }

        std::string to_string() const override
        {
            return name_;
        }

        std::unique_ptr<Expr> clone() const override
        {
            return std::make_unique<Symbol>(name_);
        }
    };

} // namespace latex_solver

#endif
