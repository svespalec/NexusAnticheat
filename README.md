# NexusAnticheat

A comprehensive anti-cheat solution designed to protect game integrity through advanced detection and prevention mechanisms.

## Project Structure

The project is organized into several components, each serving a specific purpose:

### Components

- **nexus-dll** - Core Anti-cheat Library
  - Implements the core anti-cheat functionality
  - Handles memory scanning and protection
  - Uses MinHook for function hooking
  - Built as a shared library (DLL)

- **nexus-launcher** - Game Launcher
  - Handles secure game launching
  - Verifies game integrity
  - Manages anti-cheat injection
  - Built as an executable

- **nexus-game** - Test Game
  - Simple game implementation for testing
  - Demonstrates anti-cheat integration
  - Includes basic player mechanics
  - Built as an executable

- **nexus-cheat** - Test Cheat
  - Test cheat implementation
  - Used for testing detection mechanisms
  - Helps validate anti-cheat effectiveness
  - Built as a shared library (DLL)

### Build System

The project uses CMake as its build system with the following configuration:

- C++20 standard across all components
- MSVC compiler optimizations
- Static runtime linking
- MinHook as an external dependency
- Separate CMakeLists.txt for each component

### Dependencies

- **MinHook**: Function hooking library
  - Automatically fetched and built by CMake
  - Used for API hooking and interception
  - Built as a static library

## Building

### Requirements

- Visual Studio 2022
- CMake 3.15 or higher
- Windows SDK 10.0 or higher

### Build Steps

1. Clone the repository:
```bash
git clone https://github.com/yourusername/NexusAnticheat.git
cd NexusAnticheat
```

2. Open in Visual Studio:
- Open Visual Studio 2022
- File -> Open -> CMake
- Select the root CMakeLists.txt

3. Build the solution:
- Select your desired configuration (Test/Production)
- Build -> Build All

### Output

All binaries will be placed in the `bin` directory under your build folder:
- nexus-dll.dll
- nexus-launcher.exe
- nexus-game.exe
- nexus-cheat.dll

## Development

### Project Guidelines

- Use consistent code formatting
- Follow C++20 best practices
- Document public APIs
- Write unit tests for new features

### Directory Structure

```
NexusAnticheat/
├── CMakeLists.txt          # Root CMake configuration
├── external/               # External dependencies
├── nexus-dll/             # Core anti-cheat library
│   ├── CMakeLists.txt
│   └── src/
├── nexus-launcher/        # Game launcher
│   ├── CMakeLists.txt
│   └── src/
├── nexus-game/           # Test game
│   ├── CMakeLists.txt
│   └── src/
└── nexus-cheat/         # Test cheat
    ├── CMakeLists.txt
    └── src/
```
