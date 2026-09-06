cmake_minimum_required(VERSION 3.20)
project(ArtexRecovery CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(ArtexRecovery main.cpp)