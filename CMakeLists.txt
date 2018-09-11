cmake_minimum_required(VERSION 3.12)
project(snowflake)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(dep)


find_package(Boost REQUIRED COMPONENTS system)

include_directories(${Boost_INCLUDE_DIRS})

add_executable(snowflake src/main.cpp src/SnowFlake.hpp src/Server.cpp src/Server.h src/ConnectionManager.cpp src/ConnectionManager.h src/Connection.cpp src/Connection.h src/RequestParser.cpp src/RequestParser.h src/Request.h src/Header.h src/Reply.cpp src/Reply.h src/Handle.cpp src/Handle.h)
target_link_libraries(snowflake ${Boost_LIBRARIES} json)