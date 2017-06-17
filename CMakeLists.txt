cmake_minimum_required(VERSION 3.7)
project(MasterEngine)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp engine.cpp engine.h connectionManager.cpp connectionManager.h bTree.cpp bTree.h table.cpp table.h tableutils.cpp tableutils.h operationManager.h resultCode.h resultCode.cpp operationManager.cpp where.h where.cpp join.h join.cpp jsonutils.cpp jsonutils.h diskManager.cpp diskManager.h tableManager.cpp tableManager.h operationManager.cpp operationManager.h)
add_executable(MasterEngine ${SOURCE_FILES})
