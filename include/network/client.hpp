#pragma once

#include <boost/asio.hpp>
#include <filesystem>
#include "io/sfs.hpp"
#include "network/request/id.hpp"
#include "users/user.hpp"

class RH;

class Client final {
  private:
    static inline constexpr char WELCOME_MESSAGE[3]{'S', 'M', 'C'};

    boost::asio::ip::tcp::socket socket;
    bool is_connected{true};
    bool is_logged{false};
    User user{};
    SFS sfs{*this};

    void printLocation() const;
    void startCommunication();
    void sendWelcomeMessage();
    void handleNewRequest();
    void handleReadError(boost::system::error_code error_code);
    void processReceivedRequest(RequestId request);
    RH* getHandlerFromRequest(RequestId id) const;

  public:
    Client(boost::asio::ip::tcp::socket socket);

    void start();

    bool isLogged() const;
    boost::asio::ip::tcp::socket& getSocket();
    const User& getUser() const;
    SFS& getSFS();

    void setUser(const User& user);
    void setLogged(bool is_logged);
};
