#!/usr/bin/env bash

set -e

PROJECT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
BUILD_DIR="$PROJECT_DIR/build"

cmake --build "$BUILD_DIR"
qemu-system-i386 -kernel "$BUILD_DIR/rollick.elf"