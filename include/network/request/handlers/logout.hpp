#pragma once

#include "../handler.hpp"

class LogoutRH : public RH {
  public:
    LogoutRH() = default;

    void run() override;
};
