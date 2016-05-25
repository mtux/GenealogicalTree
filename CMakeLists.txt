cmake_minimum_required(VERSION 2.6)
project(genealogicaltree)

add_executable(genealogicaltree main.cpp)

install(TARGETS genealogicaltree RUNTIME DESTINATION bin)
