#pragma once

#include <string>

class EnvVarStrResolver final {
  private:
    std::string input{}, output{};
    bool is_in_var{};
    std::string current_var{};

    bool processNextChar(char c);
    bool insertEnvVar();

  public:
    EnvVarStrResolver() = default;

    bool resolve();

    void setInput(const std::string& input);
    const std::string& getOutput() const;
};
