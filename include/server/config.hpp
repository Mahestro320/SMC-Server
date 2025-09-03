#pragma once

#include "system/beg.hpp"

#include <boost/property_tree/ini_parser.hpp>
#include <boost/property_tree/ptree.hpp>

struct ConfigValues {
    uint16_t server_port{};
    std::string dirs_storage{};
    std::string dirs_logs{};
    std::string time_format{};
};

class Config final {
  private:
    boost::property_tree::ptree property_tree{};
    ConfigValues values{};

    bool loadIniFile();
    bool tryToLoadValuesFromTree();
    void loadValuesFromTree();
    void setDirsValues() const;

    template<typename T> T getValueFromTree(const std::string& key);

    Config() = default;

  public:
    static Config& getInstance();

    bool load();

    const ConfigValues& getValues() const;
};

#include "config.ipp"
