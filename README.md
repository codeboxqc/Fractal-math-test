Fractal Art Visualizer
Fractal Art is an interactive OpenGL-based application for visualizing a variety of fractals, including Mandelbrot, Koch, Sierpinski Carpet, and more. Built using SDL2 for windowing and input handling, and OpenGL 3.3 for rendering, the program supports both pixel-based and line-based fractals. Users can switch between fractals, zoom, pan, adjust colors, and save/load fractal images using keyboard controls.

Features
Supported Fractals: Over 30 fractals, including Mandelbrot, Koch, Sierpinski Carpet, Cantor Dust, Hilbert Curve, Dragon Curve, and more.
Interactive Controls:
Keys 1-9, 0, q-w, e-r, t-y, u-i, o-p, a-s, d-f, g-h, j-k, l, z, x, c, v, b, n select different fractals.
Arrow keys (Up/Down) adjust iteration count.
Keys r, g, b modify RGB color values.
Space resets the view and settings.
s saves the current fractal to fractal.ppm.
l loads a fractal from fractal.ppm.
f toggles fullscreen mode.
p saves a screenshot to screenshot.ppm.
o times fractal computation.
d displays viewport and settings.
v shows the program version.
c displays the license (MIT).
x shows credits.
h displays the help menu.
q quits the application.
Rendering:
Pixel-based fractals (e.g., Sierpinski Carpet, Cantor) use CPU multithreading for computation and texture rendering.
Line-based fractals (e.g., Koch, Dragon) use OpenGL line strips.
Mandelbrot fractal is rendered directly in the GPU via a fragment shader.
Multithreading: Uses multiple threads to compute pixel-based fractals for improved performance.
Dependencies: SDL2, GLAD, OpenGL 3.3, and standard C++ libraries.
Known Issues
Fractals 4 (Cantor), 7 (Hilbert), 8 (Sierpinski Triangle), 9 (Box), u (Moore), l (Hilbert Variant), and n (Sierpinski Square) may not render correctly due to potential issues in the math.cpp implementations or texture sampling. Debugging steps are included in the code to identify invalid values.
Requirements
C++17 or later
SDL2
OpenGL 3.3
GLAD (included as a header)
A C++ compiler (e.g., g++, MSVC, clang)
vcpkg for dependency management (optional but recommended)
Installation and Build Instructions
See the Installation section below for detailed steps to set up dependencies using vcpkg and build the project.

Usage
Run the compiled executable.
Use the keyboard controls listed above to select fractals, adjust parameters, and interact with the visualization.
Press h to view the help menu for all commands.
License
This project is licensed under the MIT License. See LICENSE for details.

Credits
 
Built with SDL2, OpenGL, and GLAD
Installation Using vcpkg
vcpkg is a C++ package manager that simplifies the installation of libraries like SDL2 and its dependencies. Below are step-by-step instructions to install all dependencies and build the project using vcpkg on Windows, Linux, or macOS.

Prerequisites
Git: To clone vcpkg and your project repository.
CMake: For building the project (optional if using a manual build system).
C++ Compiler: g++ (Linux/macOS), MSVC (Windows), or clang.
OpenGL: Ensure your system has OpenGL 3.3 support (usually provided by your GPU drivers).



git clone https://github.com/microsoft/vcpkg.git
cd\
md vcpkg
cd vcpkg
.\bootstrap-vcpkg.bat
./bootstrap-vcpkg.sh
./vcpkg integrate install
./vcpkg install sdl2:x64-windows  
sudo apt-get install libgl1-mesa-dev
git  https://github.com/codeboxqc/Fractal-math-test.git
 

fractal-art/
├── Main.cpp
├── math.h
├── math.cpp
├── glad/
│   └── glad.h
├── CMakeLists.txt (optional, see below)
└── LICENSE

GLAD Header:
Ensure glad/glad.h and the corresponding glad.c (if needed) are in the glad/ directory. If you don’t have glad.c, generate it using the GLAD online tool with OpenGL 3.3 Core Profile and include it in the project.
Step 5: Build the Project



