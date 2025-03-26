#! /bin/bash
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
cd ..
cp camera-settings.json build/ 
mkdir -p captured_images
