# Enhance

[![Build Status](https://travis-ci.org/bilbil/enhance.svg?branch=master)](https://travis-ci.org/bilbil/enhance)
[![License](https://img.shields.io/:license-bsd-blue.svg?style=round-square)](https://github.com/bilbil/enhance/blob/master/LICENSE.txt)

=======

Experimental 3D rendering and engine tools for interactive application development. This is aimed at making customizations for developing 3D applications eaiser through a common modular service oriented approach.

## Checklist:

A = Working  
B = Partial  
C = In Progress  
D = To do  

[A] Math Utilites  
[C] UI Handling  
[B] Graphics Utilities  
[B] Service Module and Infrastructure  
[C] Memory Management  
[D] Script management  

## Dependencies:

GLFW, gl3w, OpenGL, GLM, Flex, Bison  
Linux: pkg-config  
These can be obtained via their respective package manager or built with source code.

--------------------------------------------

## Building:

Linux (supported):  
make all

Mac/Windows (not yet supported)

--------------------------------------------

## Progress:

Addition of UI stream event filtering. Testing of initial version of trackball functionality:
https://youtu.be/hnAWd_QL_XY

Testing of rotation and shadow mapping:
https://youtu.be/x8cADzdhJhs

Addition of Imgui UI toolkit:
https://youtu.be/Je6lmugDkJc

Various service module types have been created and several sample implementation on top of these are being tested. Recent development have been focusing on the rendering service; initial port of older rendering code to the new infrastructure has been tested with a test stub and is currently being integrated further into a more specific service module.

Other on-going work which will make further changes to infrastructure interface will be related to message passing and routing between service modules and other long running instances. This will allow a mechanism for signaling asynchronous task status.

Current development will be using OpenGL as the rendering backend and targeted for Linux.

--------------------------------------------

## License:

See LICENSE file.
