cmake_minimum_required(VERSION 3.12.4)
project(messagecenter VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory("dependencies/json")

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_executable(messagecenter examples/main.cpp ${SOURCES})
target_link_libraries(messagecenter PRIVATE nlohmann_json::nlohmann_json)

add_compile_definitions(MC_RESOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data/")

#  message center options
# add_compile_definitions(MC_DISABLE_POST)
# add_compile_definitions(MC_DISABLE_LOWERCASE_DEFINES)