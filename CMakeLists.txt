cmake_minimum_required(VERSION 3.0.0)
project(messagecenter VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(nlohmann_json 3.2.0 REQUIRED)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_executable(messagecenter main.cpp ${SOURCES})