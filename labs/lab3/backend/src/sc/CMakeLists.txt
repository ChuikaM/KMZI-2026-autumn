cmake_minimum_required(VERSION 3.24)
project(sc)

set(CMAKE_CXX_STANDARD 20)

add_library(${PROJECT_NAME} PUBLIC
  sc.cpp
)
target_link_directories(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC structures)