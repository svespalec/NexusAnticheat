cmake_minimum_required(VERSION 3.15)
project(NexusAnticheat)

# Set C++20 standard globally
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add compiler flags
if(MSVC)
    # Remove default warning level
    string(REGEX REPLACE "/W[0-4]" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    
    # Add warning and compiler flags
    add_compile_options(/W4 /EHsc /permissive- /MP)

    # Configure build types
    set(CMAKE_CONFIGURATION_TYPES "Test;Production" CACHE STRING "" FORCE)
    
    # Test configuration (Debug with symbols)
    set(CMAKE_C_FLAGS_TEST "${CMAKE_C_FLAGS_DEBUG} /Z7")
    set(CMAKE_CXX_FLAGS_TEST "${CMAKE_CXX_FLAGS_DEBUG} /Z7")
    
    # Production configuration
    set(CMAKE_C_FLAGS_PRODUCTION "${CMAKE_C_FLAGS_RELEASE} /DNDEBUG")
    set(CMAKE_CXX_FLAGS_PRODUCTION "${CMAKE_CXX_FLAGS_RELEASE} /DNDEBUG")
endif()

# Set static runtime for all projects
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Test>:Debug>")

# Remove MD flags and add MT flags
foreach(FLAG_VAR
        CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
        CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE)
    string(REPLACE "/MD" "/MT" ${FLAG_VAR} "${${FLAG_VAR}}")
endforeach()

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# Setup external dependencies cache
set(FETCHCONTENT_BASE_DIR "${CMAKE_SOURCE_DIR}/external/cache")

# Add MinHook
include(FetchContent)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    minhook
    GIT_REPOSITORY https://github.com/TsudaKageyu/minhook.git
    GIT_TAG master
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(minhook)

# Disable warnings for MinHook
if(MSVC)
    target_compile_options(minhook PRIVATE /W0)
endif()

# Add all subprojects
add_subdirectory(nexus-dll)
add_subdirectory(nexus-game)
add_subdirectory(nexus-cheat)
add_subdirectory(nexus-launcher)
