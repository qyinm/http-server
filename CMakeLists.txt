cmake_minimum_required(VERSION 3.29)
project(http_server)

set(CMAKE_CXX_STANDARD 20)

add_executable(http_server main.cpp
        HttpMethod.cpp
        HttpMethod.h
)
