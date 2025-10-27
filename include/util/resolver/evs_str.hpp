#pragma once

#include "system/beg.hpp"

#include <string>

/*
 * EVSStrResolver (Env Var and Sysvar String Resolver)
 *
 * Modify a string and replaces vars between <> by the real value
 * Supports env var and sysvars but not the properties
 *
 * Env vars syntax:
 * <[VAR NAME]>
 *
 * Sysvars syntax:
 * <$[SYSVAR]>
 *
 * See README.md for sysvars names
 */

enum class VarType : uint8_t {
    Environment,
    Sysvar,
    Property
};

template<bool EnableErrors = true> class EVSStrResolver final {
  private:
    std::string input{}, output{};
    bool is_in_var{};
    std::string current_var{};

    void printError(const std::string& message);
    bool processNextChar(char c);
    void flushChar(char c);
    bool insertVar();
    bool insertEmptyIfEmpty(VarType var_type);
    VarType getVarType();
    bool insertSysvar();
    bool insertEnvVar();

  public:
    EVSStrResolver() = default;

    void setInput(const std::string& input);
    bool resolve();
    const std::string& getOutput() const;
};

#include "evs_str.ipp"
