# OpenGL Viewer
A simple guide for building the OpenGL Viewer.

## Environment Setup
Place your GLFW and imgui source files in the `glfw/` and `imgui/` directories, respectively, such that its LICENSE file
is at the root each the directory. 
For convenience, I have already included the sources in the .tar.gz archive.

### Windows Setup
I used MinGW UCRT64 with MSYS2 to compile with CMake. The packages installed were:
- `mingw-w64-ucrt-x86_64-gcc`
- `mingw-w64-ucrt-x86_64-cmake`
- `mingw-w64-ucrt-x86_64-ninja`
- `mingw-w64-ucrt-x86_64-glew`

```sh
pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-ninja mingw-w64-ucrt-x86_64-glew
```
For convenience.

### Linux Setup
Unfortunately due to package manager differences, I'm not certain of the exact dependency/packages you will need.
I manually link OpenGL, GLEW, and X11 in my CMakeLists file, and a cursory graphviz visualization on my Arch system includes the following deps:
#### OpenGL
- `/usr/lib/libOpenGL.so`
- `/usr/lib/libGLX.so`
#### GLU
- `/usr/lib/libGLU.so`
#### libice
- `/usr/lib/libICE.so`
#### libsm
- `/usr/lib/libSM.so`
#### X11
- `/usr/lib/libXext.so`
- `/usr/lib/libX11.so`
#### GLEW
- `/usr/lib/libGLEW.so`

```sh
pacman -S base-devel cmake ninja glew libice libsm libxrandr libxinerama xorg-server libxcursor libxi
```

I've also included a bash script `check-linux-deps.sh` that can automatically check for all of these.
I've tested it to work on both Arch and Ubuntu 22.

## Build Steps (with Ninja)
If you have `ninja` already on your PATH, you can run `build.sh` to build.
The output will be: `build/Raytracer`

## Build Steps (without Ninja)
Run `cmake -B build` to generate the CMake build files, and then run `cmake --build build` to link and compile.
As with Ninja, the output will be: `build/Raytracer`

## Usage
In order to export frames, ensure you have a directory named `framesout` on the current working directory.