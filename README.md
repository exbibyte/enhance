# Enhance

[![Build Status](https://travis-ci.org/clearlycloudy/enhance.svg?branch=master)](https://travis-ci.org/clearlycloudy/enhance)
[![License](https://img.shields.io/:license-bsd-blue.svg?style=round-square)](https://github.com/clearlycloudy/enhance/blob/master/LICENSE.txt)

=======

Experimental 3D rendering and engine tools for interactive application development. This is aimed at making customizations for developing 3D applications through a common modular service oriented approach. Overall goal include small footprint, dependency minimization, filter pipelining approach for data processing, and high parallelism.

## Brief Status:

Math Utilites  - Working, work in progress
UI Handling  - In Progress  
Graphics Utilities  - Partial, work in progress  
Service Module and Infrastructure - Partial  
Memory Management  - In Progress  
Script management  - To Do

Current work is focused on concurrent data structures, lighting, rendering pipeline, and asset model file format.
3D model format is being revised to support md5mesh and md5anim formats.

## Dependencies:

GLFW, gl3w(http://github.com/skaslev/gl3w), OpenGL 3.3+ hardware support, Flex, Bison, pkg-config
These can be obtained via package manager or built with their repositories' source code.

--------------------------------------------

## Building:

Linux (supported):  
make all

Mac/Windows (not yet supported)

--------------------------------------------

## Progress:

Addition of UI eventing and stream filtering modules. Testing of initial version of trackball functionality on top of these:
https://youtu.be/hnAWd_QL_XY

Testing of rotation and shadow mapping:
https://youtu.be/x8cADzdhJhs

Addition of Imgui UI toolkit:
https://youtu.be/Je6lmugDkJc

--------------------------------------------

## License:

See LICENSE file.
