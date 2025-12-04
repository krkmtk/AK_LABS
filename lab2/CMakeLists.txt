cmake_minimum_required(VERSION 3.16)
project(lab2_make LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(calc STATIC src/calc.cpp)
target_include_directories(calc PUBLIC ${CMAKE_SOURCE_DIR}/include)

add_executable(app src/main.cpp)
target_link_libraries(app PRIVATE calc)
