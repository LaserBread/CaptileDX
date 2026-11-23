# CaptileDX
A "remastered" version of a game I made a while back. Capture numbers on a 5x5 grid to get a high score, and avoid capturing the bombs!

## Cross-compiling for 64-bit Windows from Linux

Install CMake 3.27 or newer, Git, Ninja, and the MinGW-w64 cross-compiler (`x86_64-w64-mingw32-gcc`, `x86_64-w64-mingw32-g++`, and `x86_64-w64-mingw32-windres`). On Debian/Ubuntu, the compiler package is commonly named `gcc-mingw-w64-x86-64` (and the corresponding G++ package).

Configure and build from the repository root:

```sh
cmake -S . -B build-mingw -G Ninja \
  -DCMAKE_TOOLCHAIN_FILE=cmake/toolchains/mingw64-linux.cmake
cmake --build build-mingw
```

On the first configure, CMake fetches pinned source revisions of GLFW, GLEW, GLM, SOIL2, Dear ImGui, and miniaudio into the build tree (`build-mingw/_deps`) and builds them with the Windows-targeting MinGW compiler. OpenGL itself is provided by the Windows/MinGW `opengl32` import library; no Linux OpenGL libraries are used for the Windows executable. The resulting executable is `build-mingw/captile_dx.exe`.

The build requires network access to GitHub during the initial configure. You can pass `-DCAPTILE_FETCH_WINDOWS_DEPS=OFF` to use dependencies already installed for the target instead, but the default MinGW cross-build fetches and builds them automatically.
