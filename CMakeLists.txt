cmake_minimum_required(VERSION 3.0)

project(obs LANGUAGES CXX) 

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc)

file(GLOB source_files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} src/*.cpp)

add_executable(${PROJECT_NAME} ${source_files})

set(CMAKE_CXX_STANDARD 17)

if (MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif()

set(SFML_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/sfml/lib/cmake/SFML")

find_package(SFML COMPONENTS graphics window system REQUIRED)

target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system)
