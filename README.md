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

[C] Core utilities

[C] Service infrastructure

[D] Service Module Integration

[D] Script management

--------------------------------------------

Unit test with Catch:
https://github.com/philsquared/Catch

--------------------------------------------

Dependencies:

GLFW, gl3w, OpenGL, GLM, Flex, Bison

Linux: pkg-config

--------------------------------------------

Progress:

Testing of rotation and shadow mapping.
https://youtu.be/x8cADzdhJhs

Addition of Imgui UI toolkit:
https://youtu.be/Je6lmugDkJc

The framework is currently undergoing restructuring to implement behavioural specialization through high level policy templates. One of the eventual goal is creating a more functional style programming available for customizing certain module behaviour.

Current development will be using OpenGL as the rendering backend until reasonable stabilization of global infrastructure and initial release of full prototype.

Current development environment will be targeted towards OSX and Linux and will add support for Windows in the distant future.

--------------------------------------------

License:

See LICENSE file.
