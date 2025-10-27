#pragma once

#include "../handler.hpp"

#include <boost/asio.hpp>
#include <string>

class RunSyscmdRH final : public RH {
  private:
    std::string command{}, output{};

    bool sendBeginResponse(boost::asio::ip::tcp::socket& socket) const;
    bool readCommand(boost::asio::ip::tcp::socket& socket);
    void runCommand();
    bool sendOutput(boost::asio::ip::tcp::socket& socket) const;

  public:
    RunSyscmdRH() = default;

    void run() override;
};
