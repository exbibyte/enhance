# Enhance

[![Build Status](https://travis-ci.org/clearlycloudy/enhance.svg?branch=master)](https://travis-ci.org/clearlycloudy/enhance)
[![License](https://img.shields.io/:license-bsd-blue.svg?style=round-square)](https://github.com/clearlycloudy/enhance/blob/master/LICENSE.txt)

=======

Experimental 3D rendering and engine tools for interactive application development. This is aimed at making customizations for developing 3D applications through a common modular service oriented approach. Overall goal include small footprint, dependency minimization, filter pipelining approach for data processing, and high parallelism.

## Brief Status:

Math Utilites  - Working

UI Handling  - Work in progress

Graphics Pipeline  - Work in progress

Service Module and Infrastructure - Working

Memory Management  - Work in progress

Script management  - To Do

Current work is focused on concurrent data structures, rendering pipeline, and memory management.

## Dependencies:

GLFW, gl3w(http://github.com/skaslev/gl3w), OpenGL 3.3+ hardware support, pkg-config
These can be obtained via package manager or built with their repositories' source code.

--------------------------------------------

## Building:

Linux (supported):  
make all

Mac/Windows (not yet supported)

--------------------------------------------

## Progress:

Addition of md5mesh and md5anim file import
https://youtu.be/pxU5Jtvp9Z4

Addition of UI eventing and stream filtering modules. Testing of initial version of trackball functionality on top of these:
https://youtu.be/hnAWd_QL_XY

Testing of rotation and shadow mapping:
https://youtu.be/x8cADzdhJhs

Addition of Imgui UI toolkit:
https://youtu.be/Je6lmugDkJc

--------------------------------------------

## Model Asset Support:

md5mesh, md5anim file format

.obj format

texture map (todo)

normal map (todo)

## Rendering Support:

no shadow

shadow mapping

shadow volume (todo)

physically based light transport(todo)

--------------------------------------------

## License:

See LICENSE file.
