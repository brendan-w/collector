cmake_minimum_required(VERSION 2.8)
project(collector)

# use FindSDL2.cmake
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

# enable C++11
set(CMAKE_CXX_FLAGS "-Wall -std=c++0x -ggdb ${CMAKE_CXX_FLAGS}")

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)

include_directories(
	"src/"
	${SDL2_INCLUDE_DIR}
	${SDL2_TTF_INCLUDE_DIR}
	${SDL2_IMAGE_INCLUDE_DIR}
)

set(SOURCE_FILES
	"src/collector.cpp"
	"src/SDL_Context.cpp"
	"src/config.cpp"
	"src/utils.cpp"
	"src/texture.cpp"
	"src/text.cpp"
	"src/SDL_utils.cpp"
	"src/thumbnail.cpp"
	"src/display/display.cpp"
	"src/display/displayobject.cpp"
	"src/display/cli.cpp"
	#"src/display/info.cpp"
	#"src/display/grid.cpp"
	#"src/display/thumbs.cpp"
	"src/filestore/filestore.cpp"
	"src/filestore/file.cpp"
	"src/filestore/selector.cpp"
	"src/filestore/selection.cpp"
)

add_executable(collector ${SOURCE_FILES})

target_link_libraries(collector
	${SDL2_LIBRARY}
	${SDL2_TTF_LIBRARY}
	${SDL2_IMAGE_LIBRARY}
)
