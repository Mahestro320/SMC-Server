#include <iostream>
#include "io/directories.hpp"
#include "server/config.hpp"
#include "util/env_var_str_resolver.hpp"

Config& Config::getInstance() {
    static Config instance{};
    return instance;
}

bool Config::load() {
    std::cout << "loading config file\n";
    if (!loadIniFile() || !tryToLoadValuesFromTree()) {
        return false;
    }
    setDirsValues();
    return true;
}

bool Config::loadIniFile() {
    try {
        boost::property_tree::ini_parser::read_ini(dirs::D_CONFIG.string(), property_tree);
    } catch (const boost::property_tree::ini_parser_error& e) {
        std::cerr << "error while parsing file: " << e.what() << std::endl;
        return false;
    }
    return true;
}

bool Config::tryToLoadValuesFromTree() {
    try {
        loadValuesFromTree();
    } catch (const boost::property_tree::ptree_error& e) {
        std::cerr << "error while getting config values from tree: " << e.what() << std::endl;
        return false;
    }
    return true;
}

void Config::loadValuesFromTree() {
    values.server_port = getValueFromTree<uint16_t>("server.port");

    values.dirs_storage = getValueFromTree<std::string>("dirs.storage");
    values.dirs_logs = getValueFromTree<std::string>("dirs.logs");
}

void Config::setDirsValues() const {
    dirs::setLogs(values.dirs_logs);
    dirs::setStorage(values.dirs_storage);
}

template<typename T> T Config::getValueFromTree(const std::string& key) {
    if constexpr (std::is_same_v<T, std::string>) {
        const std::string raw_val{property_tree.get<std::string>(key)};
        EnvVarStrResolver resolver{};
        resolver.setInput(raw_val);
        if (!resolver.resolve()) {
            throw std::runtime_error{"error while resolving key " + key};
        }
        return resolver.getOutput();
    } else {
        return property_tree.get<T>(key);
    }
}

const ConfigValues& Config::getValues() const {
    return values;
}
