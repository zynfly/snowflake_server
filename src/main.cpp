#include <iostream>
#include "SnowFlake.hpp"
#include "Server.h"

int main(int argc, char **argv)
{
    Server s(128, 4, "0.0.0.0", "8080", nlohmann::json());
    s.run();
    return 0;
}

