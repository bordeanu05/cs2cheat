cmake_minimum_required(VERSION 3.0)

project(cs2cheat LANGUAGES CXX) 

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc)

file(GLOB source_files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.cpp)

add_executable(${PROJECT_NAME} ${source_files})

set(CMAKE_CXX_STANDARD 17)

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif()

target_link_libraries(${PROJECT_NAME})