cmake_minimum_required(VERSION 3.16)
project(MultithreadTextSearchCLI LANGUAGES CXX)

# Require C++17 for std::filesystem and threading utilities
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(MultithreadTextSearchCLI
        main.cpp
        searchengine.cpp
        searchengine.h
)