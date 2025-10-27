#pragma once

#include "system/beg.hpp"

class Client;

// Request handler
class RH {
  protected:
    Client* client{};

  public:
    RH() = default;

    void setClient(Client* client);
    virtual void run() = 0;
};
