#ifndef RULES_MANAGER_H
#define RULES_MANAGER_H

#include <string>
#include <vector>

class RulesManager {
private:
    std::string rulesFilePath;

public:
    RulesManager() = default;
    ~RulesManager() = default;

    void set_rules_file_path(const std::string& path) { rulesFilePath = path; }

    void load_rules_from_file(std::vector<std::string>& rules, std::vector<std::string>& tags, std::vector<std::string>& option);
    void add_rule(const std::string& rule, const std::string& tag, const std::string& option);
    bool remove_rule(const std::string& rule, const std::string& tag, const std::string& option);
    void save_rules_to_file(const std::vector<std::string>& rules, const std::vector<std::string>& tags, const std::vector<std::string>& option);
};

#endif
