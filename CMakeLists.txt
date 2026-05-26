cmake_minimum_required(VERSION 3.8)

project(nasdaq-itch-parser VERSION 0.1)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(cmake/StandartProjectSettings.cmake)

add_executable(exe main.cpp)

