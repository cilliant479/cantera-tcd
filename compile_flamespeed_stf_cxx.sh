#!/usr/bin/env bash

set -e

REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$REPO_ROOT"

source dev_env.sh

g++ samples/cxx/flamespeed/flamespeed_stf.cpp \
  -o build/samples/cxx/flamespeed/flamespeed_stf \
  -std=c++20 \
  -pthread \
  -Iinclude \
  -Ibuild/src \
  -Lbuild/lib \
  -lcantera_shared \
  -ldl \
  -Wl,-rpath,"$REPO_ROOT/build/lib"

echo "Compiled:"
echo "  build/samples/cxx/flamespeed/flamespeed_stf"
