Enhance

[![Build Status](https://travis-ci.org/bilbil/enhance.svg?branch=master)](https://travis-ci.org/bilbil/enhance)
[![License](https://img.shields.io/:license-bsd-blue.svg?style=round-square)](https://github.com/bilbil/enhance/blob/master/LICENSE.txt)

=======

Experimental 3D rendering and engine tools for interactive application development. This is aimed at making customizable functionalities for developing 3D applications eaiser through a common infrastructure (kernel-based approach).

Checklist:

A = Working
B = Partial
C = In Progress
D = To do


[A] Math utilites

[C] UI handling

[C] Graphics utilities

[C] Service infrastructure

[C] Service Module Integration

[D] Script management

--------------------------------------------

Unit test with Catch:
https://github.com/philsquared/Catch

--------------------------------------------

Dependencies:

GLFW, gl3w, OpenGL, GLM, Flex, Bison

Linux: pkg-config

These can be obtained via paackage manager or built with their respective source code.

--------------------------------------------

Progress:

Testing of rotation and shadow mapping.
https://youtu.be/x8cADzdhJhs

Addition of Imgui UI toolkit:
https://youtu.be/Je6lmugDkJc

Various service module types have been created and several sample implementation on top of these are being tested. Recent development have been focusing on the rendering service; initial port of older rendering code to the new infrastructure has being tested with a test stub and is currently being integrated further into a more specific service module.

Current development will be using OpenGL as the rendering backend and targeted for Linux.

--------------------------------------------

License:

See LICENSE file.
