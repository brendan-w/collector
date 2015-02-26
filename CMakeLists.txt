cmake_minimum_required(VERSION 2.8)
project(Collector)

# use FindSDL2.cmake
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

# Look up SDL2 and add the include directory to our include path
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})

set(SOURCE_FILES src/main.cpp)

add_executable(Collector ${SOURCE_FILES})
target_link_libraries(Collector ${SDL2_LIBRARY})
