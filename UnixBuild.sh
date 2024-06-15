#!/bin/bash
mkdir -p build
cd build
cmake -DSILVERBACK_BUILD_UNIT_TESTS=ON -DSILVERBACK_BUILD_DEMO=ON ..
cmake --build .