# glgame2d

## Installation

This project uses the CMake build system.
Clone the repository, configure and build.

```sh
git clone https://github.com/VarunVF/glgame2d.git
cd glgame2d/
mkdir build/ && cd build/
cmake ..
cmake --build .
```

When compiling on Linux, ensure that dependencies for Wayland and X11 are installed.
For example, on Debian and derivatives like Ubuntu:

```sh
sudo apt install libwayland-dev libxkbcommon-dev xorg-dev
```

## Third Party Acknowledgments

This project has basic support for loading and rendering tilemaps created with [Tiled Map Editor](https://www.mapeditor.org/) through the `Tilemap` class.
Export tilemaps and tilesets in JSON format (`.tmj` or `.tsj`).

This project uses the following libraries:
- [glfw](https://github.com/glfw/glfw) (zlib License)
- [glad](https://glad.dav1d.de/) (Public Domain/CC0)
- [stb_image](https://github.com/nothings/stb) (Public Domain/MIT License)
- [glm](https://github.com/g-truc/glm) (MIT License)
- [miniaudio](https://github.com/mackron/miniaudio) (Public Domain/MIT 0)
- [JSON for Modern C++](https://github.com/nlohmann/json) (MIT License)
