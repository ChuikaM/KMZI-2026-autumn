cmake_minimum_required(VERSION 3.24)
project(backend_src)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(cli)

target_link_libraries(${PROJECT_NAME} INTERFACE 
  cli
)