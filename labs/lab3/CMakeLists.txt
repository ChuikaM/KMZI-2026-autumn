cmake_minimum_required(VERSION 3.24)
project(cli)

set(CMAKE_CXX_STANDARD 20)

add_library(${PROJECT_NAME} PUBLIC
  main.cpp
)

target_link_directories(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PUBLIC 
  fe 
  ge
  sc 
  sha512 
  data 
  structures
)