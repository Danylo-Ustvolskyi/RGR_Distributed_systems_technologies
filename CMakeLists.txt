cmake_minimum_required(VERSION 3.16)
project(MultithreadTextSearch LANGUAGES CXX)

# Enable Qt's automatic MOC, UIC and RCC generation. This simplifies
# the build system by automatically invoking the Meta‑Object Compiler
# on header files that contain Q_OBJECT and UIC on .ui files if they
# are added later. If you decide to add resources (qrc files) this
# will also invoke rcc.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Require a modern C++ standard. CLion supports C++17/20; here we
# explicitly require C++17 because the standard library features used
# in the search engine (std::filesystem, std::thread) were
# introduced in C++17.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Widgets Concurrent REQUIRED)

add_executable(MultithreadTextSearch
        main.cpp
        mainwindow.h
        mainwindow.cpp
        searchengine.h
        searchengine.cpp
)

target_link_libraries(MultithreadTextSearch PRIVATE Qt6::Widgets Qt6::Concurrent)