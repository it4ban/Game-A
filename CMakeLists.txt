cmake_minimum_required(VERSION 3.24)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

project(GameA VERSION 0.1.0 LANGUAGES C CXX)

add_executable(${PROJECT_NAME} WIN32)
target_sources(${PROJECT_NAME} 
    PRIVATE 
        src/main.c
    PRIVATE
        FILE_SET game_headers
        TYPE HEADERS
        BASE_DIRS
           include
        FILES
            include/main.h 
            include/single_instance.h
)

if(WIN32)
    target_sources(${PROJECT_NAME} PRIVATE src/platform/single_instance_windows.c)
elseif(APPLE OR LINUX)
    target_sources(${PROJECT_NAME} PRIVATE src/platform/single_instance_posix.c)
endif()

add_subdirectory(external)
target_link_libraries(${PROJECT_NAME} PRIVATE external)