//
// Created by zynfly on 2018/9/5.
//

#ifndef SNOWFLAKE_CONNECTIONMANAGER_H
#define SNOWFLAKE_CONNECTIONMANAGER_H

#include <set>

#include "Connection.h"
#include <mutex>

class ConnectionManager
{
public:
    explicit ConnectionManager() = default;

    ConnectionManager(ConnectionManager const &) = delete;

    ConnectionManager &operator=(ConnectionManager const &) = delete;

    void start(ConnectionPtr c);

    void stop(ConnectionPtr c);

    void stopAll();

private:
    std::set<ConnectionPtr> _connections;
    std::mutex              _mutex;
};


#endif //SNOWFLAKE_CONNECTIONMANAGER_H
