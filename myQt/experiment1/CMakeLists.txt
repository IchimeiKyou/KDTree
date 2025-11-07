cmake_minimum_required(VERSION 3.22)
project(experiment1)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(experiment1
        main.cpp
        tree.h
        widget.cpp
        widget.h)
