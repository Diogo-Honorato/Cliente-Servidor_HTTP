cmake_minimum_required(VERSION 3.0.0)
project(tp VERSION 3.1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

file(GLOB SOURCES "source/*.cpp")

add_executable(tp ${SOURCES})

target_include_directories(
    tp PRIVATE  
    include 
)

target_link_libraries(tp PRIVATE)
