#!/usr/bin/env bash

PROJECT_DIR="$(cd "$(dirname "$0")/.." && pwd)"

rm -rf "$PROJECT_DIR/build"
rm -rf "$PROJECT_DIR/cmake-build-debug"

echo "Build directories cleaned."