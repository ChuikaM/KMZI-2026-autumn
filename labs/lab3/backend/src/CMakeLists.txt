cmake_minimum_required(VERSION 3.24)
project(backend_src)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(ct)
add_subdirectory(curve)
add_subdirectory(field)
add_subdirectory(protocols)
add_subdirectory(sha512)
add_subdirectory(structures)

target_link_libraries(${PROJECT_NAME} INTERFACE 
  ct
  curve
  field
  protocols
  sha512
)