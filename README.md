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

## Running Tests

Run `ctest` from the repo root:
```bash
ctest --test-dir build/
```

## Tilemaps

glgame2d can load and render tilemaps made with [Tiled Map Editor](https://www.mapeditor.org/) through the `Tilemap` class.
Export tilemaps and tilesets in JSON format (`.tmj` / `.tsj`).

## Third-Party Libraries

- [glad](https://github.com/Dav1dde/glad)
- [GLFW](https://github.com/glfw/glfw)
- [GLM](https://github.com/g-truc/glm)
- [JSON for Modern C++](https://github.com/nlohmann/json)
- [miniaudio](https://github.com/mackron/miniaudio)
- [stb_image / stb_truetype](https://github.com/nothings/stb)

Font rendering is adapted from [stb-truetype-example](https://github.com/shreyaspranav/stb-truetype-example). Tests use [Catch2](https://github.com/catchorg/Catch2).

See [THIRD_PARTY_LICENSES.md](THIRD_PARTY_LICENSES.md) for full license texts.
