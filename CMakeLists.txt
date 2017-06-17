cmake_minimum_required(VERSION 3.7)
project(MasterEngine)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp engine.cpp engine.h connectionManager.cpp connectionManager.h operationManager.h resultCode.h .idea/resultCode.cpp.cpp resultCode.cpp operationManager.cpp where.h join.h)
add_executable(MasterEngine ${SOURCE_FILES})