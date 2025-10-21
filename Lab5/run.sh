#!/bin/bash

set -e

# Remove old build cache to avoid CMake source mismatch errors
rm -rf build

# Create build directory
mkdir -p build

# Run CMake and build
cmake -S . -B build
cmake --build build

# Run the executable
if [[ -f build/Lab5 ]]; then
    ./build/Lab5
else
    echo "Executable 'Lab5' not found in build directory."
    exit 1
fi
