# Free Plugins - Down With Expensive Audio Software

A collection of free, open-source audio plugins built with VST3 and modern C++.

## Projects

### FreeAutotune
A free autotune plugin for FL Studio and other DAWs.

**Features:**
- Real-time pitch detection using Yin algorithm
- Automatic note quantization to scale
- Pitch shifting without artifacts
- Low latency (tuned for live performance)
- Visual feedback of detected vs target pitch

**Technology:**
- VST3 architecture
- JUCE framework for UI and parameter management
- SoundTouch library for high-quality pitch shifting
- Custom Yin pitch detection for accuracy

## Building

### Prerequisites
- CMake 3.15+
- C++17 compatible compiler
- JUCE (automatically downloaded via CMake)

### Build Steps
```bash
# Clone and build
git clone https://github.com/BlackManta08/freeplugins.git
cd freeplugins
mkdir build && cd build
cmake ..
cmake --build . --config Release
```

### Dependencies
- **JUCE** - UI framework and plugin hosting
- **SoundTouch** - High-quality pitch shifting
- **Yin Algorithm** - Pitch detection

## Contributing

Ideas welcome! This is a passion project to prove great plugins don't need to be expensive.

## License

TBD