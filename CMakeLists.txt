cmake_minimum_required(VERSION 3.21 FATAL_ERROR)

project(bmpio)

add_executable(bmp bmp.cc)
set_property(TARGET bmp PROPERTY CXX_STANDARD 20)
