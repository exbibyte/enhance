# Enhance

[![Build Status](https://travis-ci.org/clearlycloudy/enhance.svg?branch=master)](https://travis-ci.org/clearlycloudy/enhance)
[![License](https://img.shields.io/:license-bsd-blue.svg?style=round-square)](https://github.com/clearlycloudy/enhance/blob/master/LICENSE.txt)

=======

Experimental 3D rendering and engine tools for interactive application development. This is aimed at making customizations for developing 3D applications eaiser through a common modular service oriented approach.

## Brief Status:

Math Utilites  - Working  
UI Handling  - In Progress  
Graphics Utilities  - Partial  
Service Module and Infrastructure - Partial  
Memory Management  - In Progress  
Script management  - To Do  

## Dependencies:

GLFW, gl3w, OpenGL 3.3+, Flex, Bison  
Linux: pkg-config  
These can be obtained via their respective package manager or built with source code.

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

Various service module types have been created and several sample implementation on top of these are being tested. Recent development have been focusing on the rendering service, synchronous and asynchronous signaling for task processing and internal messaging.

--------------------------------------------

## License:

See LICENSE file.
