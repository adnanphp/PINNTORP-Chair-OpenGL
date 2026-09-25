# 🪑 PINNTORP Chair — OpenGL 3D Model Viewer

[![GitHub Repo Size](https://img.shields.io/github/repo-size/adnanphp/PINNTORP-Chair-OpenGL)](https://github.com/adnanphp/PINNTORP-Chair-OpenGL)
[![GitHub Stars](https://img.shields.io/github/stars/adnanphp/PINNTORP-Chair-OpenGL)](https://github.com/adnanphp/PINNTORP-Chair-OpenGL/stargazers)
[![GitHub Forks](https://img.shields.io/github/forks/adnanphp/PINNTORP-Chair-OpenGL)](https://github.com/adnanphp/PINNTORP-Chair-OpenGL/network)
[![OpenGL](https://img.shields.io/badge/OpenGL-3.3%2B-green?logo=opengl)](https://www.opengl.org/)
[![C++](https://img.shields.io/badge/C%2B%2B-17-blue?logo=cplusplus)](https://isocpp.org/)
[![CMake](https://img.shields.io/badge/CMake-3.10%2B-064F8C?logo=cmake)](https://cmake.org/)
[![License](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)

> An interactive OpenGL 3D model viewer that loads a Blender-exported IKEA PINNTORP chair and provides real-time translation, rotation, scaling, and reset controls.

---

## 📋 Overview

**PINNTORP Chair — OpenGL 3D Model Viewer** is a C++ computer graphics project built with **OpenGL, GLFW, GLAD, GLM, and CMake**.

The application loads a 3D chair model exported from Blender in **OBJ format** and renders it as an interactive wireframe model.

Users can manipulate the model in real time using the keyboard.

The project demonstrates fundamental computer graphics concepts including:

* 3D model loading
* OpenGL rendering
* Vertex and fragment shaders
* Coordinate transformations
* Translation
* Rotation
* Scaling
* Keyboard input
* Camera/model interaction
* OBJ geometry processing

---

## ✨ Features

* 🖼️ **OpenGL 3.3+ Rendering**
* 📦 **OBJ Model Loading**
* 🎨 **GLSL Vertex & Fragment Shaders**
* ⌨️ **Keyboard-Based Interaction**
* ↔️ **Translation in 3D Space**
* 🔄 **Real-Time Rotation**
* 🔍 **Interactive Scaling**
* 🔲 **Wireframe Rendering**
* 🏠 **Transformation Reset**
* 🧮 **GLM-Based Matrix Mathematics**
* 🛠️ **CMake Build System**
* 💻 **Linux, macOS, and Windows Support**

---

## 🏗️ Rendering Architecture

```text
                  Blender Model
                       │
                       ▼
                  OBJ / MTL Files
                       │
                       ▼
                 OBJ Loader
                       │
                       ▼
                 Vertex Data
                       │
                       ▼
              ┌─────────────────┐
              │ OpenGL Pipeline │
              └────────┬────────┘
                       │
             ┌─────────┴─────────┐
             ▼                   ▼
       Vertex Shader       Fragment Shader
             │                   │
             └─────────┬─────────┘
                       ▼
                 Wireframe Model
                       │
                       ▼
                OpenGL Window
                       ▲
                       │
                Keyboard Input
```

---

# 🎮 Controls

| Key      | Action                    |
| -------- | ------------------------- |
| **W**    | Move chair up (Y+)        |
| **S**    | Move chair down (Y−)      |
| **A**    | Move chair left (X−)      |
| **D**    | Move chair right (X+)     |
| **Q**    | Move chair backward (Z−)  |
| **E**    | Move chair forward (Z+)   |
| **R**    | Rotate chair right        |
| **F**    | Rotate chair left         |
| **↑**    | Scale chair up            |
| **↓**    | Scale chair down          |
| **Home** | Reset all transformations |

All transformations are applied interactively while the application is running.

---

# 📸 Screenshots

### Default View

<img src="Screenshots/1.png" alt="PINNTORP Chair Default View" width="600"/>

*Default wireframe view of the chair model.*

### Rotated View

<img src="Screenshots/2.png" alt="PINNTORP Chair Rotated View" width="600"/>

*Chair rotated to show a different viewing angle.*

### Scaled View

<img src="Screenshots/3.png" alt="PINNTORP Chair Scaled View" width="600"/>

*Chair enlarged using the Up Arrow key.*

### Transformed View

<img src="Screenshots/4.png" alt="PINNTORP Chair Transformed View" width="600"/>

*Combined translation and rotation.*

### Close-Up View

<img src="Screenshots/5.png" alt="PINNTORP Chair Close Up" width="600"/>

*Close-up view showing the model's wireframe geometry.*

### Multiple Transformations

<img src="Screenshots/6.png" alt="PINNTORP Chair Multiple Transformations" width="600"/>

*Combined translation, rotation, and scaling.*

---

# 📁 Project Structure

```text
PINNTORP-Chair-OpenGL/
│
├── assets/
│   └── models/
│       ├── pinntorp_chair.obj
│       ├── pinntorp_chair.mtl
│       ├── plank_flooring_02_diff_1k.jpg
│       └── plank_flooring_02_nor_gl_1k.exr
│
├── Screenshots/
│   ├── 1.png
│   ├── 2.png
│   ├── 3.png
│   ├── 4.png
│   ├── 5.png
│   └── 6.png
│
├── src/
│   ├── main.cpp
│   ├── obj_loader.hpp
│   └── obj_loader.cpp
│
├── shaders/
│   ├── vertex_shader.glsl
│   └── fragment_shader.glsl
│
├── glad/
│   └── # GLAD OpenGL loader
│
├── include/
│   └── # External headers
│
├── build/
│   └── # Generated build directory
│
├── build.sh
├── CMakeLists.txt
├── LICENSE
└── README.md
```

> The `build/` directory is generated during compilation and normally should not be committed to Git.

---

# 🚀 Quick Start

## Prerequisites

You need:

* C++17-compatible compiler
* CMake 3.10+
* OpenGL 3.3+
* GLFW 3.3+
* GLM 0.9.9+
* GLAD
* Git

---

## 🐧 Ubuntu / Debian

Install the required packages:

```bash
sudo apt update

sudo apt install \
    build-essential \
    cmake \
    libglfw3-dev \
    libglm-dev \
    libgl1-mesa-dev
```

Check your OpenGL version:

```bash
glxinfo | grep "OpenGL version"
```

If `glxinfo` is unavailable:

```bash
sudo apt install mesa-utils
```

---

## 🍎 macOS

Using Homebrew:

```bash
brew install cmake glfw glm
```

---

## 🪟 Windows

Using vcpkg:

```bash
vcpkg install glfw3 glm
```

A MinGW or Visual Studio C++ toolchain is also required.

---

# 🔨 Build and Run

## 1. Clone the Repository

```bash
git clone https://github.com/adnanphp/PINNTORP-Chair-OpenGL.git
cd PINNTORP-Chair-OpenGL
```

## 2. Make the Build Script Executable

```bash
chmod +x build.sh
```

## 3. Build

```bash
./build.sh
```

## 4. Run

```bash
cd build
./chair_app
```

---

# 🧱 Manual CMake Build

If you prefer to build without `build.sh`:

```bash
mkdir -p build
cd build

cmake ..
make -j$(nproc)
```

Run:

```bash
./chair_app
```

---

# ⚙️ Manual g++ Build

On Linux, the project can also be compiled directly with `g++`:

```bash
g++ -std=c++17 \
    src/main.cpp \
    src/obj_loader.cpp \
    -I/usr/include/glm \
    -lglfw \
    -lGL \
    -ldl \
    -o chair_app
```

Run:

```bash
./chair_app
```

---

# 🌍 Platform Builds

### Linux

```bash
./build.sh
cd build
./chair_app
```

### macOS

```bash
./build.sh
cd build
./chair_app
```

### Windows

Using CMake with MinGW:

```bash
cmake -B build -G "MinGW Makefiles"
cmake --build build
```

Run:

```bash
cd build
./chair_app.exe
```

---

# 🧰 Dependencies

| Dependency | Version | Purpose                       |
| ---------- | ------- | ----------------------------- |
| **OpenGL** | 3.3+    | Graphics rendering            |
| **GLFW**   | 3.3+    | Window creation and input     |
| **GLAD**   | —       | OpenGL function loading       |
| **GLM**    | 0.9.9+  | Vector and matrix mathematics |
| **CMake**  | 3.10+   | Build configuration           |
| **C++**    | C++17   | Application development       |

---

# 📐 3D Transformations

The application supports the three fundamental model transformations:

### Translation

Moves the chair along the X, Y, and Z axes.

```text
X → left / right
Y → up / down
Z → forward / backward
```

### Rotation

Rotates the model around its vertical axis.

### Scaling

Increases or decreases the model size while preserving its proportions.

### Reset

The **Home** key restores the model to its initial transformation state.

---

# 📦 OBJ Model Support

The custom OBJ loader supports:

* ✅ Vertex positions
* ✅ Vertex normals
* ✅ UV coordinates
* ✅ Face definitions
* ✅ Triangulated geometry

The associated MTL file supports:

* ✅ Material definitions
* ✅ Texture references

The current application renders the model in **wireframe mode**, so textures are not required for the displayed result.

---

# 🔄 Updating the 3D Model

You can replace the chair with another OBJ model.

### 1. Export from Blender

Export your model as:

```text
OBJ
```

### 2. Copy the Files

Place the model files in:

```text
assets/models/
```

For example:

```text
assets/models/my_model.obj
assets/models/my_model.mtl
```

### 3. Update the Model Path

If necessary, update the model filename in:

```text
src/main.cpp
```

### 4. Rebuild

```bash
./build.sh
```

---

# ✅ Computer Graphics Requirements

The project implements the following core requirements:

* ☑ OpenGL rendering window
* ☑ GLFW window and input handling
* ☑ GLAD OpenGL function loading
* ☑ 3D OBJ model loading
* ☑ Vertex and fragment shaders
* ☑ Translation transformations
* ☑ Rotation transformations
* ☑ Scaling transformations
* ☑ Keyboard interaction
* ☑ Wireframe rendering
* ☑ Reset functionality
* ☑ Blender-exported 3D model

---

# 🛠️ Troubleshooting

## `Failed to open OBJ file`

Verify that the model exists:

```bash
ls -la assets/models/
```

Make sure:

```text
assets/models/pinntorp_chair.obj
```

exists and that you are running the application from the expected project/build directory.

---

## `Shader compilation failed`

Verify the shader files:

```text
shaders/
├── vertex_shader.glsl
└── fragment_shader.glsl
```

Also check for GLSL syntax errors.

---

## `GLFW/GLAD initialization failed`

Check that OpenGL is available:

```bash
glxinfo | grep "OpenGL"
```

Also verify that your graphics drivers and required development packages are installed.

---

## Build Script Fails

Make sure the script is executable:

```bash
chmod +x build.sh
```

Check CMake:

```bash
cmake --version
```

If required, install the build dependencies:

```bash
sudo apt install build-essential cmake libglfw3-dev libglm-dev libgl1-mesa-dev
```

---

# 🎓 Educational Objectives

This project provides practical experience with:

* **Computer graphics programming**
* **OpenGL rendering pipelines**
* **3D transformations**
* **Shader programming**
* **Model loading**
* **Linear algebra**
* **C++ development**
* **GLFW event handling**
* **CMake build systems**
* **Interactive graphical applications**

---

# 🤝 Contributing

Contributions are welcome.

1. Fork the repository.
2. Create a feature branch:

```bash
git checkout -b feature/AmazingFeature
```

3. Commit your changes:

```bash
git commit -m "Add AmazingFeature"
```

4. Push your branch:

```bash
git push origin feature/AmazingFeature
```

5. Open a Pull Request.

---

# 📄 License

This project is released under the **MIT License**.

See [LICENSE](LICENSE) for details.

---

# 🙏 Credits

### 3D Model

**IKEA PINNTORP Chair**

### Software

* Blender 4.3.2
* OpenGL 3.3+

### Libraries

* GLFW
* GLAD
* GLM

### Textures

`plank_flooring_02` — free PBR texture asset included with the project.

---

# 👨‍💻 Author

**Muhammad Adnan Shahzad**

GitHub: [@adnanphp](https://github.com/adnanphp)

**Project:**
https://github.com/adnanphp/PINNTORP-Chair-OpenGL

---

<div align="center">

### 🪑 3D Graphics × C++ × OpenGL

**Interactive Rendering • 3D Transformations • Shader Programming**

</div>
