#pragma once

#include "system/beg.hpp"

#include <boost/asio.hpp>
#include <filesystem>
#include "io/sfs.hpp"
#include "network/request/id.hpp"
#include "users/user.hpp"

class RH;

class Client final {
  private:
    static inline constexpr char WELCOME_MESSAGE[3]{'S', 'M', 'C'};

    static thread_local std::unique_ptr<Client> thread_instance;

    boost::asio::ip::tcp::socket socket;
    uint32_t index{};

    bool is_connected{true};
    bool is_logged{false};
    User user{};
    SFS sfs{*this};

    boost::asio::ip::tcp::endpoint getRemoteEndpoint() const;
    void printLocation() const;
    void startCommunication();
    void sendWelcomeMessage();
    void handleNewRequest();
    void handleReadError(boost::system::error_code error_code);
    void processReceivedRequest(RequestId request);
    RH* getHandlerFromRequest(RequestId id) const;

  public:
    explicit Client(boost::asio::ip::tcp::socket socket, uint32_t index);

    static void setInstance(boost::asio::ip::tcp::socket socket, uint32_t index);
    static Client& getInstance();
    static bool isInstanceSet();

    void start();

    bool isLogged() const;
    boost::asio::ip::tcp::socket& getSocket();
    const User& getUser() const;
    SFS& getSFS();
    uint32_t getIndex() const;
    uint16_t getPort() const;
    std::string getAddress() const;

    void setUser(const User& user);
    void setLogged(bool is_logged);
};
