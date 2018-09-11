//
// Created by zynfly on 2018/9/6.
//

#ifndef SNOWFLAKE_HANDLE_H
#define SNOWFLAKE_HANDLE_H

#include <nlohmann/json.hpp>
#include <unordered_map>
#include <string>
#include "SnowFlake.hpp"
#include "Request.h"
#include "Reply.h"

class Handle
{
public:
    explicit Handle(nlohmann::json config);

    Handle(Handle const &) = delete;

    Handle &operator=(Handle const &) = delete;

    void handleRequest(Request const &req, Reply &rep);

private:
    std::unordered_map<std::string, std::shared_ptr<SnowFlakeInterface>> _snow_flake_map;
};


#endif //SNOWFLAKE_HANDLE_H
