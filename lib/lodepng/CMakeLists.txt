cmake_minimum_required(VERSION 3.10)
project(lodepng C)

add_library(lodepng STATIC lodepng.c)
target_include_directories(lodepng PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
