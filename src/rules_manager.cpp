#include "rules_manager.h"

#include <cstddef>
#include <filesystem>
#include <fstream>
#include <stdexcept>

#include <nlohmann/json.hpp>

namespace {

using json = nlohmann::json;

void validate_option(const std::string& option) {
    if (option != "permit" && option != "block") {
        throw std::invalid_argument("Rule option must be either 'permit' or 'block', got: " + option);
    }
}

void validate_rule(const std::string& rule, const std::string& tag, const std::string& option) {
    if (rule.empty())
        throw std::invalid_argument("Rule value cannot be empty");
    if (tag.empty())
        throw std::invalid_argument("Rule tag cannot be empty");
    validate_option(option);
}

} // namespace

void RulesManager::load_rules_from_file(std::vector<std::string>& rules, std::vector<std::string>& tags, std::vector<std::string>& option) {
    if (rulesFilePath.empty())
        throw std::runtime_error("Rules file path has not been set");

    std::ifstream input(rulesFilePath);
    if (!input)
        throw std::runtime_error("Cannot open rules file: " + rulesFilePath);

    json document;
    try {
        input >> document;
    } catch (const json::exception& error) {
        throw std::runtime_error("Invalid rules JSON in " + rulesFilePath + ": " + error.what());
    }

    if (!document.is_array())
        throw std::runtime_error("Rules JSON root must be an array: " + rulesFilePath);

    std::vector<std::string> loadedRules;
    std::vector<std::string> loadedTags;
    std::vector<std::string> loadedOptions;
    loadedRules.reserve(document.size());
    loadedTags.reserve(document.size());
    loadedOptions.reserve(document.size());

    for (std::size_t index = 0; index < document.size(); ++index) {
        const json& entry = document[index];
        if (!entry.is_object() ||
            !entry.contains("rule") || !entry["rule"].is_string() ||
            !entry.contains("tag") || !entry["tag"].is_string() ||
            !entry.contains("option") || !entry["option"].is_string()) {
            throw std::runtime_error("Invalid rule entry at index " + std::to_string(index));
        }

        const std::string rule = entry["rule"].get<std::string>();
        const std::string tag = entry["tag"].get<std::string>();
        const std::string ruleOption = entry["option"].get<std::string>();

        try {
            validate_rule(rule, tag, ruleOption);
        } catch (const std::invalid_argument& error) {
            throw std::runtime_error("Invalid rule entry at index " + std::to_string(index) + ": " + error.what());
        }

        loadedRules.push_back(rule);
        loadedTags.push_back(tag);
        loadedOptions.push_back(ruleOption);
    }

    // Do not partially modify caller-owned vectors when parsing fails.
    rules = std::move(loadedRules);
    tags = std::move(loadedTags);
    option = std::move(loadedOptions);
}

void RulesManager::add_rule(const std::string& rule, const std::string& tag, const std::string& option) {
    if (rulesFilePath.empty())
        throw std::runtime_error("Rules file path has not been set");

    validate_rule(rule, tag, option);

    std::vector<std::string> rules;
    std::vector<std::string> tags;
    std::vector<std::string> options;

    if (std::filesystem::exists(rulesFilePath))
        load_rules_from_file(rules, tags, options);

    rules.push_back(rule);
    tags.push_back(tag);
    options.push_back(option);
    save_rules_to_file(rules, tags, options);
}

bool RulesManager::remove_rule(const std::string& rule, const std::string& tag, const std::string& option) {
    std::vector<std::string> rules;
    std::vector<std::string> tags;
    std::vector<std::string> options;
    load_rules_from_file(rules, tags, options);

    for (std::size_t index = 0; index < rules.size(); ++index) {
        if (rules[index] == rule && tags[index] == tag && options[index] == option) {
            rules.erase(rules.begin() + static_cast<std::ptrdiff_t>(index));
            tags.erase(tags.begin() + static_cast<std::ptrdiff_t>(index));
            options.erase(options.begin() + static_cast<std::ptrdiff_t>(index));
            save_rules_to_file(rules, tags, options);
            return true;
        }
    }
    return false;
}

void RulesManager::save_rules_to_file(const std::vector<std::string>& rules, const std::vector<std::string>& tags, const std::vector<std::string>& option) {
    if (rulesFilePath.empty())
        throw std::runtime_error("Rules file path has not been set");
    if (rules.size() != tags.size() || rules.size() != option.size()) {
        throw std::invalid_argument("Rules, tags and options must contain the same number of items");
    }

    json document = json::array();
    for (std::size_t index = 0; index < rules.size(); ++index) {
        validate_rule(rules[index], tags[index], option[index]);
        document.push_back({
            {"rule", rules[index]},
            {"tag", tags[index]},
            {"option", option[index]}
        });
    }

    const std::filesystem::path filePath(rulesFilePath);
    if (filePath.has_parent_path()) {
        std::error_code error;
        std::filesystem::create_directories(filePath.parent_path(), error);
        if (error) {
            throw std::runtime_error("Cannot create rules directory: " + filePath.parent_path().string());
        }
    }

    std::ofstream output(rulesFilePath, std::ios::trunc);
    if (!output)
        throw std::runtime_error("Cannot write rules file: " + rulesFilePath);

    output << document.dump(2) << '\n';
    if (!output)
        throw std::runtime_error("Failed while writing rules file: " + rulesFilePath);
}
