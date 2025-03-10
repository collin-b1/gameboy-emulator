cmake_minimum_required(VERSION 3.10)
project(Gobby)

# Specify C++ Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include vcpkg
if(DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
else()
    message(FATAL_ERROR "Please define VCPKG_ROOT environment variable.")
endif()

find_package(SDL2 CONFIG REQUIRED)

# Include directories
include_directories(include)

# Set ROMs directory path
set(ROMS_DIRECTORY "${CMAKE_SOURCE_DIR}/assets/roms/")

# Pass the ROMs directory to the code
add_definitions(-DROMS_DIRECTORY="${ROMS_DIRECTORY}")

# Add source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Define the executable
add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)