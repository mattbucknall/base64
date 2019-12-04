cmake_minimum_required(VERSION 2.8)

project(base64 C)

set(CMAKE_C_STANDARD 99)

add_library(base64 STATIC base64.c)
target_include_directories(base64 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(test main.c)
target_link_libraries(test PUBLIC base64)

add_subdirectory(tests)
