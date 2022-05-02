#!/bin/bash

CWD=$(pwd)

# Pull GIT Submodules
git submodule update --init --recursive

# Activate Emscripten
cd $CWD/emsdk
./emsdk install 3.1.9
./emsdk activate 3.1.9
source emsdk_env.sh

# Build Zlib
cd $CWD/zlib
mkdir build
cd build
emcmake cmake ../ -DCMAKE_BUILD_TYPE=Release
emmake make -j

# Build libpng
cd $CWD/libpng
mkdir build
cd build
emcmake cmake ../ \
-DCMAKE_BUILD_TYPE=Release \
-DZLIB_LIBRARY="$CWD/zlib/build/libz.a" \
-DZLIB_INCLUDE_DIR="$CWD/zlib/;$CWD/zlib/build"
emmake make -j

# Build Zint
cd $CWD/zint
mkdir build
cd build
emcmake cmake ../ \
-DCMAKE_BUILD_TYPE=Release \
-DZLIB_LIBRARY="$CWD/zlib/build/libz.a" \
-DZLIB_INCLUDE_DIR="$CWD/zlib/;$CWD/zlib/build" \
-DPNG_LIBRARY="$CWD/libpng/build/libpng.a" \
-DPNG_PNG_INCLUDE_DIR="$CWD/libpng/;$CWD/libpng/build"
emmake make -j