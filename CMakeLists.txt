cmake_minimum_required(VERSION 3.15)
project(FreePlugins VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# JUCE Configuration
add_subdirectory(JUCE)

# SoundTouch library
find_package(SoundTouch REQUIRED)

# FreeAutotune Plugin
juce_add_plugin(FreeAutotune
    PRODUCT_NAME "FreeAutotune"
    COMPANY_NAME "FreePlugins"
    PLUGIN_MANUFACTURER_CODE "FePl"
    PLUGIN_CODE "FrAt"
    NEEDS_MIDI_INPUT TRUE
    NEEDS_MIDI_OUTPUT FALSE
    IS_SYNTH FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
    VST3_CATEGORIES "Audio"
    AU_MAIN_TYPE "kAudioUnitType_Effect"
)

target_sources(FreeAutotune PRIVATE
    src/PluginProcessor.cpp
    src/PluginEditor.cpp
    src/PitchDetector.cpp
    src/PitchShifter.cpp
)

target_include_directories(FreeAutotune PRIVATE
    src
    ${SOUNDTOUCH_INCLUDE_DIR}
)

target_link_libraries(FreeAutotune PRIVATE
    juce::juce_audio_basics
    juce::juce_audio_devices
    juce::juce_audio_formats
    juce::juce_audio_processors
    juce::juce_audio_utils
    juce::juce_core
    juce::juce_data_structures
    juce::juce_events
    juce::juce_graphics
    juce::juce_gui_basics
    juce::juce_gui_extra
    ${SOUNDTOUCH_LIBRARIES}
)