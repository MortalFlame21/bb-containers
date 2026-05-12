cmake_minimum_required(VERSION 3.15...4.0.0)
project(bb_containers LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED 20)

enable_testing()
add_subdirectory(test)