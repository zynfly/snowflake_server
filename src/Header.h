//
// Created by zynfly on 2018/9/5.
//

#ifndef SNOWFLAKE_HEADER_H
#define SNOWFLAKE_HEADER_H

#include <string>

struct Header
{
    std::string name;
    std::string value;

    Header() = default;

    Header(std::string const &n, std::string const &v)
    {
        name  = n;
        value = v;
    }
};


#endif //SNOWFLAKE_HEADER_H
