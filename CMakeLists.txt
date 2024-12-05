cmake_minimum_required(VERSION 3.11)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(StationOutbreak)

#### Setup Directories ####
SET(OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIRECTORY})

#### Add External Dependencies ####
add_subdirectory("lib/sfml")
set(SFML_INCS "lib/sfml/include")
link_directories("${CMAKE_BINARY_DIR}/lib/sfml/lib")

#### Game ####
file(GLOB_RECURSE SOURCES game/*.cpp game/*.h)
add_executable(GAME ${SOURCES})
target_include_directories(GAME PRIVATE ${SFML_INCS})
target_link_libraries(GAME sfml-graphics)