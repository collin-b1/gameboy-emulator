cmake_minimum_required(VERSION 3.22)
project(Gobby)

# Specify C++ Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set(CMAKE_AUTOMOC ON) # For Qt meta-object compiler
# set(CMAKE_AUTORCC ON) # If using Qt resources
# set(CMAKE_AUTOUIC ON) # If using .ui files

if (MSVC)
    add_compile_options(/Zc:__cplusplus)
endif ()

## Include vcpkg
#if(DEFINED ENV{VCPKG_ROOT})
#    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
#else()
#    message(FATAL_ERROR "Please define VCPKG_ROOT environment variable.")
#endif()

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

qt_standard_project_setup()

# Include directories
include_directories(include)

# Add source files
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h" "include/*.hpp")

# Define the executable
qt_add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
)