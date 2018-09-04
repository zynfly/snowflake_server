cmake_minimum_required(VERSION 3.12)
project(snowflake)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(dep)


add_executable(snsowflake src/main.cpp src/snowflake.cpp src/snowflake.h)