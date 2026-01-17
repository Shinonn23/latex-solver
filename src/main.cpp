#include "core/ast/equation.h"
#include "core/ast/expr.h"
#include "core/common/context.h"
#include "core/eval/evaluator.h"
#include "core/parser/parser.h"
#include <iostream>
#include <memory>
#include <string>

using namespace std;
using namespace latex_solver;

void print_help() {
    cout << "\n=== LaTeX Solver - Basic Calculator ===\n";
    cout << "Console commands:\n";
    cout << "  <expression>       - Evaluate expression\n";
    cout << "  set <var> <value>  - Set variable value\n";
    cout << "  vars               - Show all variables\n";
    cout << "  clear              - Clear all variables\n";
    cout << "  help               - Show this help\n";
    cout << "  exit               - Exit program\n";
    cout << "\n";
}

void print_usage(const char *program_name) {
    cout << "Flags commands:\n";
    cout << "Usage: " << program_name << " [OPTIONS]\n";
    cout << "  --help, -h              Show this help message\n";
    cout << "  --version, -v           Show version information\n";
    cout << "  --eval <expr>           Evaluate expression and exit\n";
    cout << "  --set <var> --value <n> Set variable value\n";
}

void handle_set_command(const string &input, Context &ctx) {
    size_t pos = 3;
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

void handle_expression(const string &input, Context &ctx) {
    try {
        Parser parser(input);
        auto   expr = parser.parse();

        cout << "Expression: " << expr->to_string() << endl;

        Evaluator eval(ctx);
        double    result = eval.evaluate(*expr);

        cout << "Result: " << result << endl;
    } catch (const exception &e) {
        cout << e.what() << endl;
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

int main(int argc, char *argv[]) {
    Context ctx;

    // Handle command-line arguments
    if (argc > 1) {
        bool   has_eval = false;
        string eval_expr;

        for (int i = 1; i < argc; ++i) {
            string arg = argv[i];

            if (arg == "--help" || arg == "-h") {
                print_help();
                print_usage(argv[0]);
                return 0;
            } else if (arg == "--version" || arg == "-v") {
                cout << "LaTeX Solver v1.0\n";
                return 0;
            } else if (arg == "--set" && i + 1 < argc) {
                string var_name = argv[++i];
                if (i + 1 < argc && string(argv[i + 1]) == "--value" &&
                    i + 2 < argc) {
                    ++i; // skip --value
                    string value_str = argv[++i];
                    try {
                        double value = stod(value_str);
                        ctx.set(var_name, value);
                        cout << "Set " << var_name << " = " << value << endl;
                    } catch (...) {
                        cout << "Error: Invalid value for " << var_name << "\n";
                        return 1;
                    }
                } else {
                    cout << "Error: --set requires --value <number>\n";
                    print_usage(argv[0]);
                    return 1;
                }
            } else if (arg == "--eval" && i + 1 < argc) {
                eval_expr = argv[++i];
                has_eval  = true;
            } else {
                cout << "Unknown option: " << arg << "\n";
                print_usage(argv[0]);
                return 1;
            }
        }

        if (has_eval) {
            handle_expression(eval_expr, ctx);
            return 0;
        }

        // If only variables were set, show them and exit
        if (argc > 1) {
            return 0;
        }
    }

    cout << "LaTeX Solver v1.0 - Basic Calculator\n";
    cout << "Type 'help' for commands, 'exit' to quit.\n";

    string input;
    while (true) {
        cout << "\n> ";
        if (!getline(cin, input)) {
            break;
        }

        size_t start = input.find_first_not_of(" \t");
        if (start == string::npos)
            continue;
        input = input.substr(start);

        if (input.empty())
            continue;

        if (input == "exit" || input == "quit") {
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
        } else {
            handle_expression(input, ctx);
        }
    }

    return 0;
}