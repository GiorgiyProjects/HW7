cmake_minimum_required(VERSION 3.5)

project(bayan)

find_package(Boost COMPONENTS unit_test_framework REQUIRED)
add_executable(bayan main.cpp)

set_target_properties(bayan PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED ON)
