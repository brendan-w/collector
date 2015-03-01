cmake_minimum_required(VERSION 2.8)
project(Collector)

# use FindSDL2.cmake
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

# enable C++11
set(CMAKE_CXX_FLAGS "-Wall -std=c++0x ${CMAKE_CXX_FLAGS}")

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)

include_directories(
	${SDL2_INCLUDE_DIR}
	${SDL2_TTF_INCLUDE_DIR}
	${SDL2_IMAGE_INCLUDE_DIR}
)

set(SOURCE_FILES
	"src/collector.cpp"
	"src/display.cpp"
	"src/cli.cpp"
	"src/filestore.cpp"
	"src/texture.cpp"
	"src/config.cpp"
)

add_executable(Collector ${SOURCE_FILES})

target_link_libraries(Collector
	${SDL2_LIBRARY}
	${SDL2_TTF_LIBRARY}
	${SDL2_IMAGE_LIBRARY}
)
