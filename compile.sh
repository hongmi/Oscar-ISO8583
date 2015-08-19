#!/bin/sh

## build the project

mkdir debug release

echo "build debug..."
cd debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
make

echo "build release..."
cd release
cmake ..
make
