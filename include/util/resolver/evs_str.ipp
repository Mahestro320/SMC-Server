#pragma once

#include "common.hpp"
#include "io/console.hpp"
#include "server/sysvar.hpp"

template<bool EnableErrors>
void EVSStrResolver<EnableErrors>::printError(const std::string& message) {
    if constexpr (EnableErrors) {
        console::out::err(message);
    }
}

template<bool EnableErrors>
bool EVSStrResolver<EnableErrors>::processNextChar(char c) {
    if (c == '<') {
        if (is_in_var && EnableErrors) {
            printError("syntax error: two var opening found");
            return false;
        }
        is_in_var = true;
        return true;
    }
    if (c == '>') {
        if (!is_in_var && EnableErrors) {
            printError("syntax error: close of var found without opening");
            return false;
        }
        is_in_var = false;
        if (!insertVar()) {
            return false;
        }
        current_var.clear();
        return true;
    }
    flushChar(c);
    return true;
}

template<bool EnableErrors>
void EVSStrResolver<EnableErrors>::flushChar(char c) {
    if (is_in_var) {
        current_var += c;
    } else {
        output += c;
    }
}

template<bool EnableErrors>
bool EVSStrResolver<EnableErrors>::insertVar() {
    const VarType var_type{getVarType()};
    if (insertEmptyIfEmpty(var_type)) {
        return !EnableErrors;
    }
    if (var_type == VarType::Sysvar) {
        return insertSysvar();
    } else if (var_type == VarType::Environment) {
        return insertEnvVar();
    }
    output += "<" + current_var + '>';
    return true;
}

template<bool EnableErrors>
bool EVSStrResolver<EnableErrors>::insertEmptyIfEmpty(VarType var_type) {
    if (current_var.empty() || (current_var.size() == 1 && var_type != VarType::Property)) {
        if constexpr (EnableErrors) {
            printError("syntax error: empty env var");
        } else {
            output +=
                "<" +
                std::string{(var_type == VarType::Property) ? ":" : ((var_type == VarType::Sysvar) ? "$" : "")} +
                ">";
        }
        return true;
    }
    return false;
}

template<bool EnableErrors>
VarType EVSStrResolver<EnableErrors>::getVarType() {
    if (current_var.empty() || current_var.starts_with(":")) {
        return VarType::Property;
    } else if (current_var.starts_with("$")) {
        return VarType::Sysvar;
    }
    return VarType::Environment;
}

template<bool EnableErrors>
bool EVSStrResolver<EnableErrors>::insertSysvar() {
    std::string sysvar_val{sysvar::get(current_var.substr(1))};
    if (sysvar_val.empty()) {
        if constexpr (EnableErrors) {
            printError("unknown sysval: " + current_var.substr(1));
            return false;
        }
        output += common::UNDEFINED_TEXT;
        return true;
    }
    output += sysvar_val;
    return true;
}

template<bool EnableErrors>
bool EVSStrResolver<EnableErrors>::insertEnvVar() {
    char* env_var_val{};
    size_t size{};
    if (_dupenv_s(&env_var_val, &size, current_var.c_str()) != 0 || !env_var_val) {
        if constexpr (EnableErrors) {
            printError("error: unknown env var: " + current_var);
            return false;
        }
        output += '<' + current_var + '>';
        return true;
    }
    output += env_var_val;
    free(env_var_val);
    return true;
}

template<bool EnableErrors>
bool EVSStrResolver<EnableErrors>::resolve() {
    for (char c : input) {
        if (!processNextChar(c)) {
            output = input;
            return false;
        }
    }
    return true;
}

template<bool EnableErrors>
void EVSStrResolver<EnableErrors>::setInput(const std::string& input) {
    this->input = input;
}

template<bool EnableErrors>
const std::string& EVSStrResolver<EnableErrors>::getOutput() const {
    return output;
}
