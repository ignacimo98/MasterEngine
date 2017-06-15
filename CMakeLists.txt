cmake_minimum_required(VERSION 3.7)
project(MasterEngine)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp engine.cpp engine.h connectionManager.cpp connectionManager.h)
add_executable(MasterEngine ${SOURCE_FILES})