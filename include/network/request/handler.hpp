#pragma once

#include "network/client.hpp"

// Request handler
class RH {
  protected:
    Client* client{};

  public:
    RH() = default;

    void setClient(Client* client);
    virtual void run() = 0;
};
