#ifndef NUMBER_H
#define NUMBER_H

#include "expr.h"
#include <string>

namespace latex_solver {

class Number : public Expr {
  private:
	double value_;

  public:
	explicit Number(double value) : value_(value) {}

	double value() const { return value_; }

	void accept(ExprVisitor &visitor) const override { visitor.visit(*this); }

	std::string to_string() const override {
		std::string str = std::to_string(value_);
		// Remove trailing zeros
		// etc value_ = 3.1400000 -> "3.14"
		str.erase(str.find_last_not_of('0') + 1, std::string::npos); // npos is a static member constant value
		if (str.back() == '.') {
			str.pop_back();
		}
		return str;
	}

	std::unique_ptr<Expr> clone() const override {
		return std::make_unique<Number>(value_);
	}
};

} // namespace latex_solver

#endif
