#pragma once

#include "network/client.hpp"

// RequestHandler
class RH {
  protected:
    Client* client{};

  public:
    RH() = default;

    void setClient(Client* client);
    virtual void run() = 0;
};
