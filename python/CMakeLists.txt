cmake_minimum_required(VERSION 3.14)
project(bakkesmod-bindings LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Find pybind11
find_package(pybind11 REQUIRED)

# SDK paths
set(SDK_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../include")
set(SDK_LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../lib")

# Source files
set(BINDING_SOURCES
    bindings/main.cpp
    bindings/core_structs.cpp
    bindings/enums.cpp
    bindings/engine_wrappers.cpp
    bindings/game_event_wrappers.cpp
    bindings/game_object_wrappers.cpp
    bindings/game_wrapper.cpp
    bindings/wrappers.cpp
    bindings/items_wrappers.cpp
    bindings/misc_wrappers.cpp
)

# Create the Python extension module
pybind11_add_module(_bindings ${BINDING_SOURCES})

# Include directories
target_include_directories(_bindings PRIVATE ${SDK_INCLUDE_DIR})

# Link against the BakkesMod SDK library
if(WIN32)
    target_link_libraries(_bindings PRIVATE ${SDK_LIB_DIR}/pluginsdk.lib)
else()
    # Cross-compilation or alternative linking
    message(WARNING "BakkesModSDK is Windows-only. Build on Windows with MSVC.")
endif()

# Install to the bakkesmod package directory
install(TARGETS _bindings DESTINATION bakkesmod)

# Copy __init__.py to the build directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/bakkesmod/__init__.py
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bakkesmod/)
