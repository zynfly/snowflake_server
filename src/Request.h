//
// Created by zynfly on 2018/9/5.
//

#ifndef SNOWFLAKE_REQUEST_H
#define SNOWFLAKE_REQUEST_H

#include <string>
#include <vector>
#include <boost/asio.hpp>
#include "Header.h"

struct Request
{
    std::string                    method;
    std::string                    uri;
    uint32_t                       http_version_major;
    uint32_t                       http_version_minor;
    std::vector<Header>            headers;
    boost::asio::ip::tcp::endpoint remote_endpoint;
};


#endif //SNOWFLAKE_REQUEST_H
