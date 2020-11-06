cmake_minimum_required(VERSION 3.5)
project(test)

find_package(Boost REQUIRED)
find_package(fmt REQUIRED)

add_executable(test main.cpp)

