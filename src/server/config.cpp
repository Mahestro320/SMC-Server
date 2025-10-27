#include "server/config.hpp"

#include <iostream>
#include "io/directories.hpp"
#include "termcolor.hpp"

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
        std::cerr << termcolor::red << "error while parsing file: " << e.what() << termcolor::reset << std::endl;
        return false;
    }
    return true;
}

bool Config::tryToLoadValuesFromTree() {
    try {
        loadValuesFromTree();
    } catch (const boost::property_tree::ptree_error& e) {
        std::cerr << termcolor::red << "error while getting config values from tree: " << e.what() << termcolor::reset
                  << std::endl;
        return false;
    }
    return true;
}

void Config::loadValuesFromTree() {
    values.server_port = getValueFromTree<uint16_t>("server.port");

    values.dirs_storage = getValueFromTree<std::string>("dirs.storage");
    values.dirs_logs = getValueFromTree<std::string>("dirs.logs");

    values.time_format = getValueFromTree<std::string>("time.format");
}

void Config::setDirsValues() const {
    dirs::setLogs(values.dirs_logs);
    dirs::setStorage(values.dirs_storage);
}

const ConfigValues& Config::getValues() const {
    return values;
}
