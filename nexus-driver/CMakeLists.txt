cmake_minimum_required(VERSION 3.15)
project(nexus-driver)

# Set WDK and SDK paths - adjust these according to your installation
set(WDK_ROOT "C:/Program Files (x86)/Windows Kits/10")
set(WDK_VERSION "10.0.26100.0")  # Updated to match your installed version

# Disable manifest generation
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /MANIFEST:NO")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /MANIFEST:NO")

# Disable RTTI first (must be before /kernel)
string(REPLACE "/GR" "/GR-" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

# Set compiler flags for kernel driver
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /kernel /driver")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GS- /W4 /Zi")

# Architecture definitions for kernel mode
add_definitions(
    -D_AMD64_=1
    -DAMD64=1
    -D_WIN64=1
    -DWIN64=1
    -D_KERNEL_MODE
    -D_WIN32_WINNT=0x0A00
)

# Collect source files
file(GLOB_RECURSE SOURCES 
    "src/*.cpp"
    "src/*.hpp"
    "src/*.h"
)

# Create driver target
add_library(${PROJECT_NAME} SHARED ${SOURCES})

# Set driver properties
set_target_properties(${PROJECT_NAME} PROPERTIES 
    SUFFIX ".sys"
    LINK_FLAGS "/DRIVER /SUBSYSTEM:NATIVE /ENTRY:DriverEntry /NODEFAULTLIB /MACHINE:X64"
)

# Add WDK include paths
target_include_directories(${PROJECT_NAME} PRIVATE
    "${WDK_ROOT}/Include/${WDK_VERSION}/km/crt"
    "${WDK_ROOT}/Include/${WDK_VERSION}/km"
    "${WDK_ROOT}/Include/${WDK_VERSION}/shared"
)

# Link WDK libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    "${WDK_ROOT}/Lib/${WDK_VERSION}/km/x64/BufferOverflowK.lib"
    "${WDK_ROOT}/Lib/${WDK_VERSION}/km/x64/ntoskrnl.lib"
    "${WDK_ROOT}/Lib/${WDK_VERSION}/km/x64/hal.lib"
    "${WDK_ROOT}/Lib/${WDK_VERSION}/km/x64/wmilib.lib"
)
