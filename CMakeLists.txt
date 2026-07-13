cmake_minimum_required(VERSION 3.24)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(GameA VERSION 0.1.0 LANGUAGES C CXX)

add_executable(${PROJECT_NAME} WIN32)
target_sources(${PROJECT_NAME} PRIVATE src/main.c)

add_subdirectory(external PRIVATE)
target_link_libraries(${PROJECT_NAME} PRIVATE external)