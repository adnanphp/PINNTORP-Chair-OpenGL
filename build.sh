

#!/bin/bash

# build.sh - Build script for the OpenGL Chair project
# Usage: ./build.sh [clean]

set -e  # Exit on error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

PROJECT_NAME="opengl_chair_project"
BUILD_DIR="build"

# Print colored message
print_message() {
    echo -e "${GREEN}[INFO]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

# Check for clean command
if [ "$1" = "clean" ]; then
    print_message "Cleaning build directory..."
    if [ -d "$BUILD_DIR" ]; then
        rm -rf "$BUILD_DIR"
        print_message "Build directory removed."
    else
        print_warning "Build directory does not exist."
    fi
    exit 0
fi

# Check if dependencies are installed
print_message "Checking dependencies..."

# Check for cmake
if ! command -v cmake &> /dev/null; then
    print_error "CMake not found. Please install: sudo apt install cmake"
    exit 1
fi

# Check for g++
if ! command -v g++ &> /dev/null; then
    print_error "g++ not found. Please install: sudo apt install build-essential"
    exit 1
fi

# Check for GLFW
if ! pkg-config --exists glfw3; then
    print_warning "GLFW3 not found. Please install: sudo apt install libglfw3-dev"
    print_warning "Continuing anyway..."
fi

# Check for GLM
if [ ! -d "/usr/include/glm" ] && [ ! -d "/usr/local/include/glm" ]; then
    print_warning "GLM not found. Please install: sudo apt install libglm-dev"
    print_warning "Continuing anyway..."
fi

# Check for GLAD files
if [ ! -f "include/glad/glad.h" ]; then
    print_error "GLAD header not found: include/glad/glad.h"
    print_error "Please generate GLAD from https://glad.dav1d.de/"
    print_error "Settings: Language=C/C++, Specification=OpenGL, API=gl 3.3 Core"
    print_error "Then copy include/ and src/glad.c to this project"
    exit 1
fi

if [ ! -f "src/glad.c" ]; then
    print_error "GLAD source not found: src/glad.c"
    print_error "Please copy glad.c from the GLAD download"
    exit 1
fi

# Create build directory
if [ ! -d "$BUILD_DIR" ]; then
    mkdir -p "$BUILD_DIR"
    print_message "Created build directory: $BUILD_DIR"
fi

# Navigate to build directory
cd "$BUILD_DIR"

# Run CMake configuration
print_message "Running CMake configuration..."
cmake ..

# Build the project
print_message "Building the project..."
make -j$(nproc)

# Check if build succeeded
if [ $? -eq 0 ]; then
    print_message "Build completed successfully!"
    print_message "Run the application: ./chair_app"
    echo ""
    print_message "Controls:"
    echo "  W/A/S/D - Translate on X/Y axes"
    echo "  Q/E     - Translate on Z axis (forward/backward)"
    echo "  R/F     - Rotate around Y axis"
    echo "  Up/Down - Scale up/down"
    echo "  Home    - Reset all transformations"
else
    print_error "Build failed!"
    exit 1
fi
