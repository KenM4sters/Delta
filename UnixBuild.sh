#!/bin/bash
mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug  -S ../ -B .
make
cd ..