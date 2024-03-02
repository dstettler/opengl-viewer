rm -rf build
cmake -G Ninja -B build/debug -DCMAKE_BUILD_TYPE=Debug
ninja -C build/debug

cmake -G Ninja -B build/release -DCMAKE_BUILD_TYPE=Release
ninja -C build/release