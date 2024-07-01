cmake_minimum_required( VERSION 3.28 FATAL_ERROR )

include( FetchContent )

FetchContent_Declare( json
    GIT_REPOSITORY https://github.com/nlohmann/json
    GIT_TAG v3.11.3
    EXCLUDE_FROM_ALL
)

FetchContent_MakeAvailable( json )