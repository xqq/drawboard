drawboard
======
Final project for Exercise on Operating Systems

## Features
- TCP-based online multiplayer drawing board
- Cross platform, support Windows / Linux / macOS
- Random pencil color
- Support undo operation

## Preparing
```bash
git clone https://github.com/xqq/drawboard.git
cd drawboard
git submodule update --init
```

### Windows build (mingw-w64 x86_64)
- Install msys2 & necessary packages
```bash
pacman -S mingw-w64-x86_64-toolchain mingw-w64-x86_64-cmake mingw-w64-x86_64-SDL2
```

### Windows build (MSVC)
- Install packages by using vcpkg
```bash
vcpkg install sdl2:x86-windows
vcpkg install sdl2:x64-windows
```

### macOS build
- Use homebrew package manager
```bash
brew install cmake sdl2
```

### Linux build (Debian-like distributions)
```bash
sudo apt-get install cmake libsdl2-dev
```

## Build
### MSVC Build
```bash
mkdir build
cd build
cmake .. -G "Visual Studio 15" -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=<PATH_TO_VCPKG>\vcpkg\scripts\buildsystems\vcpkg.cmake
cmake --build
```

### Other toolchains
```bash
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j8
```

Or using CLion (recommended)

## Usage
```bash
Usage: drawboard [OPTIONS]
       --client <host> --port <port>
       --server <host> --port <port>
```

Start a drawboard server
```bash
./drawboard -s 0.0.0.0 -p 1919
```

Connect to a drawboard server
```bash
./drawboard -c 127.0.0.1 -p 1919
```

## License
```
MIT License

Copyright (c) 2020 xqq <xqq@xqq.im>
```