#pragma once

#include "../handler.hpp"

class GetUserRoleRH final : public RH {
  public:
    GetUserRoleRH() = default;

    void run() override;
};
