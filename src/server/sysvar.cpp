#include "server/sysvar.hpp"

#include "common.hpp"
#include "network.hpp"
#include "network/client.hpp"
#include "server/config.hpp"
#include "util/string.hpp"

namespace {

bool getFromConfig(std::string& var, const std::string& name) {
    const Config& config{Config::getInstance()};
    const ConfigValues& config_values{config.getValues()};
    if (name == "port") {
        var = std::to_string(config_values.server_port);
    } else if (name == "time_format") {
        var = config_values.time_format;
    } else if (name == "logs_path") {
        var = config_values.dirs_logs;
    } else if (name == "storage_path") {
        var = config_values.dirs_storage;
    } else {
        return false;
    }
    return true;
}

bool getFromUserNoInstance(std::string& var, const std::string& name) {
    if (name == "username" || name == "userrole") {
        var = common::UNDEFINED_TEXT;
        return true;
    }
    return false;
}

bool getFromUser(std::string& var, const std::string& name) {
    if (!Client::isInstanceSet()) {
        return getFromUserNoInstance(var, name);
    }
    const Client& client{Client::getInstance()};
    const User& user{client.getUser()};
    if (name == "username") {
        var = user.name;
    } else if (name == "userrole") {
        var = role::getName(user.role);
    } else {
        return false;
    }
    return true;
}

bool getFromGlobals(std::string& var, const std::string& name) {
    if (name == "version") {
        var = common::VERSION.toString();
    } else if (name == "address") {
        var = network::getIpv4Address();
    } else {
        return false;
    }
    return true;
}

bool getFromClientNoInstance(std::string& var, const std::string& name) {
    if (name == "clt_index" || name == "clt_logged" || name == "clt_port" || name == "clt_address") {
        var = common::UNDEFINED_TEXT;
        return true;
    }
    return false;
}

bool getFromClient(std::string& var, const std::string& name) {
    if (!Client::isInstanceSet()) {
        return getFromClientNoInstance(var, name);
    }
    Client& client{Client::getInstance()};
    if (name == "clt_index") {
        var = client.getIndex();
    } else if (name == "clt_logged") {
        var = std::to_string(client.isLogged());
    } else if (name == "clt_port") {
        var = std::to_string(client.getPort());
    } else if (name == "clt_address") {
        var = client.getAddress();
    } else {
        return false;
    }
    return true;
}

} // namespace

std::string sysvar::get(std::string name) {
    std::string value{};
    getFromConfig(value, name);
    getFromUser(value, name);
    getFromGlobals(value, name);
    getFromClient(value, name);
    return value;
}
