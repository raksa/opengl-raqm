#!/bin/sh

# raw compile
# clang++ -w -framework glut -framework opengl src/main_opengl.cpp -o main

cmake CMakeLists.txt && make && ./main ../Battambang-Regular.ttf "សោះស្តី" "l" "km"