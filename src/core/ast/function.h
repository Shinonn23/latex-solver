#ifndef FUNCTION_H
#define FUNCTION_H

#include "expr.h"
#include <memory>
#include <stdexcept>
#include <string>

namespace latex_solver {
class Function : public Expr {
  private:
	std::string name_; // Function name (e.g., "sqrt", "sin")
	ExprPtr argument_; // Function argument

  public:
	Function(const std::string &name, ExprPtr argument)
		: name_(name), argument_(std::move(argument)) {
		if (!argument_) {
			throw std::invalid_argument("Function: argument cannot be null");
		}
	}

	const std::string &name() const { return name_; }
	const Expr &argument() const { return *argument_; }

	void accept(ExprVisitor &visitor) const override { visitor.visit(*this); }

	std::string to_string() const override {
		return name_ + "(" + argument_->to_string() + ")";
	}

	std::unique_ptr<Expr> clone() const override {
		return std::make_unique<Function>(name_, argument_->clone());
	}
};

} // namespace latex_solver

#endif
