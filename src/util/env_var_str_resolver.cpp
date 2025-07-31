#include "io/console.hpp"
#include "util/env_var_str_resolver.hpp"

bool EnvVarStrResolver::resolve() {
    for (char c : input) {
        if (!processNextChar(c)) {
            return false;
        }
    }
    return true;
}

bool EnvVarStrResolver::processNextChar(char c) {
    if (c == '<') {
        if (is_in_var) {
            console::out::err("syntax error: two env var opening found");
            return false;
        }
        is_in_var = true;
        return true;
    }
    if (c == '>') {
        if (!is_in_var) {
            console::out::err("syntax error: close of env var found without opening");
            return false;
        }
        is_in_var = false;
        return insertEnvVar();
    }
    if (is_in_var) {
        current_var += c;
    } else {
        output += c;
    }
    return true;
}

bool EnvVarStrResolver::insertEnvVar() {
    if (current_var.empty()) {
        console::out::err("syntax error: empty env var");
        return false;
    }
    char* env_var_val{};
    size_t size{};
    if (_dupenv_s(&env_var_val, &size, current_var.c_str()) != 0 || !env_var_val) {
        console::out::err("error: unknown env var: " + current_var);
        return false;
    }
    output += env_var_val;
    free(env_var_val);
    return true;
}

void EnvVarStrResolver::setInput(const std::string& input) {
    this->input = input;
}

const std::string& EnvVarStrResolver::getOutput() const {
    return output;
}
