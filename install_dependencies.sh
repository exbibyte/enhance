#!/bin/sh

echo "installing dependencies..."

sudo apt install -y pkg-config xorg-dev libgl1-mesa-dev

echo "downloading and installing http://github.com/skaslev/gl3w"
git clone http://github.com/skaslev/gl3w.git
mkdir temp && gl3w/gl3w_gen.py temp
mkdir core/impl/render/gl/GL
cp temp/include/GL/* core/impl/render/gl/GL/
cp temp/src/* core/impl/render/gl/GL/
rm -r -f temp gl3w

echo "downloading and installing http://github.com/glfw/glfw"
git clone http://github.com/glfw/glfw.git
cd glfw && cmake CMakeLists.txt && make && make install && cd ../ && rm -r -f glfw

echo "finished."
