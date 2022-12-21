cmake_minimum_required(VERSION 3.18)

project(cpp_currying LANGUAGES CXX)

include_directories("${CMAKE_SOURCE_DIR}")
add_executable(devtest "${CMAKE_SOURCE_DIR}/devtest.cc")

set_target_properties(devtest PROPERTIES CXX_STANDARD 17)
