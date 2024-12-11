# NexusAnticheat

An anti-cheat solution designed to protect game integrity through detection and prevention mechanisms.

## Components

- **nexus-dll**: Core anti-cheat library (DLL)
- **nexus-launcher**: Game launcher and integrity verifier
- **nexus-game**: Test game implementation
- **nexus-cheat**: Test cheat for validation

## Build Requirements

- Visual Studio 2022
- CMake 3.15+
- Windows SDK 10.0+

## Building

### Option 1: Visual Studio (Recommended)
1. Open the project folder in Visual Studio 2022
2. VS will automatically configure CMake
3. Select your configuration (Test/Production)
4. Build -> Build All

### Option 2: CMake Command Line
```bash
cmake -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Production
```

## Project Structure

```
NexusAnticheat/
├── CMakeLists.txt
├── external/           # Dependencies (MinHook)
├── nexus-dll/         # Core anti-cheat
├── nexus-launcher/    # Game launcher
├── nexus-game/        # Test game
└── nexus-cheat/       # Test cheat
```

Build outputs will be in `bin/` directory.
