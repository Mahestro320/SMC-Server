#pragma once

#include "system/beg.hpp"

#include "../handler.hpp"

class GetVersionRH final : public RH {
  public:
    GetVersionRH() = default;

    void run() override;
};
