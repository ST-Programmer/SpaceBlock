CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
project(SpaceBlock)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(${PROJECT_NAME} src/main.cpp src/game.cpp src/entity.cpp src/player.cpp src/tile.cpp src/plant.cpp)

find_package(SDL3 REQUIRED)

set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
target_include_directories(${PROJECT_NAME} 
	PRIVATE
		${INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME} SDL3::SDL3)
