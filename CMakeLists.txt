cmake_minimum_required(VERSION 3.29)
project(http_server)

set(CMAKE_CXX_STANDARD 20)

# Rapidjson 추가
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/third_party/rapidjson/include)

add_executable(http_server main.cpp
        HttpMethod.cpp
        HttpMethod.h
)
