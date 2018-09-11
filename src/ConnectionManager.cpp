//
// Created by zynfly on 2018/9/5.
//

#include "ConnectionManager.h"


void ConnectionManager::start(ConnectionPtr c)
{
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _connections.insert(c);
    }
    c->start();
}

void ConnectionManager::stop(ConnectionPtr c)
{
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _connections.erase(c);
    }
    c->stop();
}

void ConnectionManager::stopAll()
{
    for (auto &&c:_connections) {
        c->stop();
    }

    _connections.clear();
}
