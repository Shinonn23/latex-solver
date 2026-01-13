#ifndef CONTEXT_H
#define CONTEXT_H

#include <string>
#include <unordered_map>
#include <optional>

namespace latex_solver
{

    // Context for storing variable values
    class Context
    {
    private:
        std::unordered_map<std::string, double> variables_;

    public:
        void set(const std::string &name, double value)
        {
            variables_[name] = value;
        }

        std::optional<double> get(const std::string &name) const
        {
            auto it = variables_.find(name);
            if (it != variables_.end())
            {
                return it->second;
            }
            return std::nullopt;
        }

        const std::unordered_map<std::string, double> &get_all() const
        {
            return variables_;
        }

        bool has(const std::string &name) const
        {
            return variables_.find(name) != variables_.end();
        }

        void clear()
        {
            variables_.clear();
        }

        size_t size() const
        {
            return variables_.size();
        }
    };

} // namespace latex_solver

#endif // CONTEXT_H
