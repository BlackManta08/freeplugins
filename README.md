# FreeAutotune - Free Autotune Plugin for FL Studio

Down with expensive plugins! **FreeAutotune** is an open-source pitch correction plugin for FL Studio built with [iPlug2](https://github.com/iPlug2/iPlug2).

## ✨ Features

- Real-time pitch correction
- Visual editor for note adjustment
- Low-latency audio processing
- Cross-platform build support (Windows + Linux)
- Open-source and free to use

## 📁 Project Structure

- `src/` — C++ source code for DSP and UI
- `CMakeLists.txt` — CMake build configuration
- `build.sh` — Unix/macOS build helper script
- `build_linux.sh` — Linux-specific build helper script
- `build.bat` — Windows build helper script
- `PROJECT_STATUS.md` — Current roadmap and project progress
- `IMPLEMENTATION_NOTES.md` — Development and architecture notes

## 🧱 Requirements

To build this project, you need:

- CMake (3.15+ recommended)
- A C++17 compatible compiler
- iPlug2 (with dependencies initialized)
- Platform build tools:
  - Windows: Visual Studio Build Tools
  - Linux: `build-essential`, X11/OpenGL dev libs (as required by iPlug2)

## 🔧 Build Instructions

### Windows

```bat
build.bat
```

### Linux

```bash
chmod +x build_linux.sh
./build_linux.sh
```

### Generic Unix

```bash
chmod +x build.sh
./build.sh
```

## 🚀 Usage

After building, load the generated plugin binary in FL Studio (or another supported VST host).

> Note: exact output paths depend on your iPlug2 + CMake configuration.

## 🗺️ Current Status

See [PROJECT_STATUS.md](./PROJECT_STATUS.md) for completed work and remaining TODOs.

## 🧪 Recommended Additions (Still Missing)

To make this project easier for contributors and users, consider adding:

1. **LICENSE** file (high priority)  
   Clearly defines how others can use and distribute your code.
2. **CONTRIBUTING.md**  
   Documents coding standards, branch naming, PR flow, and testing expectations.
3. **GitHub Actions CI workflow** (e.g., `.github/workflows/build.yml`)  
   Runs build checks on pushes/PRs.
4. **Releases/Artifacts section**  
   Publish built binaries so non-developers can test without compiling.
5. **Screenshots or GIFs** in README  
   Helps users quickly understand UI and functionality.

## 🤝 Contributing

Contributions are welcome. Please open an issue or pull request to discuss improvements.

## 📌 Disclaimer

This is an independent open-source project and is not affiliated with FL Studio/Image-Line.
