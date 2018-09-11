//
// Created by zynfly on 2018/9/5.
//

#ifndef SNOWFLAKE_CONNECTION_H
#define SNOWFLAKE_CONNECTION_H


#include <memory>
#include <boost/asio.hpp>
#include "RequestParser.h"
#include "Reply.h"
#include "Handle.h"

class ConnectionManager;

class Connection : public std::enable_shared_from_this<Connection>
{
public:
    explicit Connection(boost::asio::ip::tcp::socket socket, ConnectionManager &manager, Handle &handle);

    Connection(Connection const &) = delete;

    Connection &operator=(Connection const &) = delete;

    void start();

    void stop();

    void doRead();

    void doWrite();

private:
    boost::asio::ip::tcp::socket _socket;
    ConnectionManager            &_connection_manager;
    std::array<char, 8192>       _buffer;
    RequestParser                _parser;
    Request                      _request;
    Reply                        _reply;
    Handle                       &_handle;

};


typedef std::shared_ptr<Connection> ConnectionPtr;


#endif //SNOWFLAKE_CONNECTION_H
