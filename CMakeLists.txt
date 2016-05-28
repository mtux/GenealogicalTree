cmake_minimum_required(VERSION 2.6)
project(genealogicaltree)

add_executable(genealogicaltree treefileloader.cpp treefilegenerator.cpp genealogicaltree.cpp prompthandler.cpp main.cpp)

set_property(TARGET genealogicaltree PROPERTY CXX_STANDARD 14)

install(TARGETS genealogicaltree RUNTIME DESTINATION bin)
