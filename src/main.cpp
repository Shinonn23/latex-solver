#include "core/ast/binary.h"
#include "core/ast/expr.h"
#include "core/ast/number.h"
#include "core/common/context.h"
#include "core/common/evaluator.h"
#include "core/common/parser.h"
#include "core/rewrite/simplify.h"
#include <iostream>
#include <memory>
#include <string>

using namespace std;
using namespace latex_solver;

void print_help() {
	cout << "\n=== LaTeX Solver - Basic Calculator ===\n";
	cout << "Commands:\n";
	cout << "  <expression>       - Evaluate expression (e.g., 2 + 3 * 4)\n";
	cout << "  simplify <expr>    - Simplify expression\n";
	cout << "  set <var> <value>  - Set variable value (e.g., set x 5)\n";
	cout << "  vars               - Show all variables\n";
	cout << "  clear              - Clear all variables\n";
	cout << "  help               - Show this help\n";
	cout << "  exit               - Exit program\n";
	cout << "\nSupported operations: + - * /\n";
	cout << "Examples:\n";
	cout << "  > 2 + 3\n";
	cout << "  > 10 * (5 + 3)\n";
	cout << "  > set x 10\n";
	cout << "  > x * 2 + 5\n";
	cout << "  > simplify x + 0\n\n";
}

void handle_set_command(const string &input, Context &ctx) {
	// Parse: set <var> <value>
	size_t pos = 3; // skip "set"
	while (pos < input.size() && isspace(input[pos]))
		pos++;

	size_t var_start = pos;
	while (pos < input.size() && !isspace(input[pos]))
		pos++;
	string var_name = input.substr(var_start, pos - var_start);

	while (pos < input.size() && isspace(input[pos]))
		pos++;
	string value_str = input.substr(pos);

	try {
		double value = stod(value_str);
		ctx.set(var_name, value);
		cout << "Set " << var_name << " = " << value << endl;
	} catch (...) {
		cout << "Error: Invalid value. Usage: set <variable> <number>\n";
	}
}

void handle_simplify_command(const string &input) {
	// Parse: simplify <expression>
	size_t pos = 8; // skip "simplify"

	// etc input = "simplify   2 + 0 "
	// input.size() = 17
	while (pos < input.size() && isspace(input[pos]))
		pos++;

	// Now pos points to the start of the expression
	// etc input.substr(11) = "2 + 0 "
	string expr_str = input.substr(pos);

	try {
		Parser parser(expr_str);
		auto expr = parser.parse();

		cout << "Original: " << expr->to_string() << endl;

		auto simplified = Simplifier::simplify(*expr);
		cout << "Simplified: " << simplified->to_string() << endl;
	} catch (const exception &e) {
		cout << "Error: " << e.what() << endl;
	}
}

void handle_expression(const string &input, Context &ctx) {
	try {
		Parser parser(input);
		auto expr = parser.parse();

		cout << "Expression: " << expr->to_string() << endl;

		Evaluator eval(ctx);
		double result = eval.evaluate(*expr);

		cout << "Result: " << result << endl;
	} catch (const exception &e) {
		cout << "Error: " << e.what() << endl;
	}
}

void show_variables(const Context &ctx) {
	if (ctx.size() == 0) {
		cout << "No variables defined.\n";
	} else {
		cout << "Variables:\n";
		for (const auto &pair : ctx.get_all()) {
			cout << "  " << pair.first << " = " << pair.second << "\n";
		}
	}
}

int main() {
	Context ctx;

	cout << "LaTeX Solver v1.0 - Basic Calculator\n";
	cout << "Type 'help' for commands, 'exit' to quit.\n";

	string input;
	while (true) {
		cout << "\n> ";
		if (!getline(cin, input)) {
			break;
		}

		// Trim whitespace
		size_t start = input.find_first_not_of(" \t");
		if (start == string::npos)
			continue;
		input = input.substr(start);

		if (input.empty())
			continue;

		// Handle commands
		if (input == "exit" || input == "quit") {
			// cout << "Goodbye!\n";
			break;
		} else if (input == "help") {
			print_help();
		} else if (input == "clear") {
			ctx.clear();
			cout << "Variables cleared.\n";
		} else if (input == "vars") {
			show_variables(ctx);
		} else if (input.substr(0, 3) == "set" && input.size() > 3 &&
				   isspace(input[3])) {
			handle_set_command(input, ctx);
		} else if (input.substr(0, 8) == "simplify" &&
				   (input.size() == 8 || isspace(input[8]))) {
			handle_simplify_command(input);
		} else {
			handle_expression(input, ctx);
		}
	}

	return 0;
}