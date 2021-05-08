# chess
singleplayer and multiplayer chess

assets from [here](https://gitlab.com/zulban/chesscraft-creative-commons/-/tree/master)

# install dependencies
```vcpkg install asio sdl2 sdl2-image sdl2-ttf```

# building
```
mkdir build
cd build
cmake .. "-DCMAKE_TOOLCHAIN_FILE=path_to_vcpkg/scripts/buildsystems/vcpkg.cmake"
```
