//
// Created by zynfly on 2018/9/5.
//

#ifndef SNOWFLAKE_SERVER_H
#define SNOWFLAKE_SERVER_H

#include <cstdint>
#include <string>
#include <boost/asio.hpp>

#include "ConnectionManager.h"

class Server
{
public:
    explicit Server(uint32_t max_connection, uint32_t thread_num, std::string const &address, std::string const &port,nlohmann::json config);

    Server(const Server &) = delete;

    Server &operator=(Server const &) = delete;

    void run();

private:
    void doAccept();

private:
    uint32_t                       _thread_num;
    boost::asio::io_context        _io_context;
    boost::asio::ip::tcp::acceptor _acceptor;

    ConnectionManager _connection_manager;
    Handle            _handle;
};


#endif //SNOWFLAKE_SERVER_H
