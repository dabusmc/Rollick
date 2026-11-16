#!/usr/bin/env bash

set -e

PROJECT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
BUILD_DIR="$PROJECT_DIR/build"

cmake -S "$PROJECT_DIR" -B "$BUILD_DIR" \
    -G Ninja \
    -DCMAKE_TOOLCHAIN_FILE="$PROJECT_DIR/cmake/toolchain-i686-elf.cmake" \
    -DCMAKE_BUILD_TYPE=Debug

cmake --build "$BUILD_DIR"