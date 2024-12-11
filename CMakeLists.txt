cmake_minimum_required(VERSION 3.8)

project("NexusAnticheat")

# Set C++20 standard for all projects
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add compiler flags
if(MSVC)
    # Remove default warning level
    string(REGEX REPLACE "/W[0-4]" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    
    # Add our warning and compiler flags
    add_compile_options(/W4 /EHsc /permissive-)

    # Configure build types
    set(CMAKE_CONFIGURATION_TYPES "Test;Production" CACHE STRING "" FORCE)
    
    # Test configuration (Debug with symbols)
    set(CMAKE_C_FLAGS_TEST "${CMAKE_C_FLAGS_DEBUG} /Z7")
    set(CMAKE_CXX_FLAGS_TEST "${CMAKE_CXX_FLAGS_DEBUG} /Z7")
    
    # Production configuration (Release with no debug info)
    set(CMAKE_C_FLAGS_PRODUCTION "${CMAKE_C_FLAGS_RELEASE} /DNDEBUG")
    set(CMAKE_CXX_FLAGS_PRODUCTION "${CMAKE_CXX_FLAGS_RELEASE} /DNDEBUG")
endif()

# Set static runtime for all projects
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Test>:Debug>")

# Remove the /MD and /MDd flags before adding /MT
string(REPLACE "/MD" "" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
string(REPLACE "/MDd" "" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
string(REPLACE "/MD" "" CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}")
string(REPLACE "/MDd" "" CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}")

# Add /MT flags
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MT")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MTd")

# Set output directory for all targets
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# Add MinHook
include(FetchContent)

# Setup external dependencies cache
set(FETCHCONTENT_BASE_DIR "${CMAKE_SOURCE_DIR}/external/cache")

# Configure MinHook
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE) # Force static build
FetchContent_Declare(
    minhook
    GIT_REPOSITORY https://github.com/TsudaKageyu/minhook.git
    GIT_TAG master
    GIT_SHALLOW TRUE
)

# Make MinHook available
FetchContent_MakeAvailable(minhook)

# Disable warnings for MinHook
if(MSVC)
    target_compile_options(minhook PRIVATE /W0)
endif()

# Find all source files for the executable
file(GLOB_RECURSE EXE_SOURCES 
    "src/exe/*.cpp"
    "src/exe/*.hpp"
    "src/exe/*.h"
)

# Find all source files for the DLL
file(GLOB_RECURSE DLL_SOURCES
    "src/dll/*.cpp"
    "src/dll/*.hpp"
    "src/dll/*.h"
)

# Executable project
add_executable(NexusAnticheat ${EXE_SOURCES})

# DLL project
add_library(NexusAnticheatDLL SHARED ${DLL_SOURCES})

# Add include directories
target_include_directories(NexusAnticheatDLL PRIVATE 
    ${CMAKE_SOURCE_DIR}/src/dll
    ${minhook_SOURCE_DIR}/include
)

# Link MinHook - using the correct target name
target_link_libraries(NexusAnticheatDLL PRIVATE minhook)
