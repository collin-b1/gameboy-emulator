cmake_minimum_required(VERSION 3.10)
project(Gobby)

# Specify C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(SDL2 REQUIRED)

# Include directories
include_directories(include)

# Set ROMs directory path
set(ROMS_DIRECTORY "${CMAKE_SOURCE_DIR}/assets/roms/")

# Pass the ROMs directory to the code
add_definitions(-DROMS_DIRECTORY="${ROMS_DIRECTORY}")

# Add source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Define the executable
add_executable(Gobby ${SOURCES})

# Link SDL2
target_link_libraries(Gobby SDL2::SDL2 SDL2::SDL2main)