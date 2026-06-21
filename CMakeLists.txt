cmake_minimum_required(VERSION 3.10)
project(game-engine C CXX)

set(CMAKE_CXX_STANDARD 17)

option(ENABLE_COVERAGE "Enable code coverage" OFF)
if (ENABLE_COVERAGE)
    message(STATUS "Code coverage enabled")
    add_compile_options(--coverage -O0 -g)
    add_link_options(--coverage)
endif()

add_subdirectory(vendor)
add_subdirectory(src)

enable_testing()
add_subdirectory(tests)