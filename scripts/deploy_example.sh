#!/bin/bash

rm -rf build/
cmake -S . -B build/
cd build ; make; cd ..
./build/example/CRYSTAL_HOME