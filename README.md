# Enhance/e2

Minimalist 3D rendering and game engine developed in C++.

Focus of the engine to provide tools to create customizable engine kernels and game modules, independence from external libraries, parition of rendering and game logic layers, multithreaded task based progamming model.

Certain parts of C++ are leveraged such as templates and namespaces. Meanwhile other parts of C++ are avoided: implicit function invokations and expensive copy constructors.

# Current Progress

The engine is being revised in core modules and multithreading facilities.

Currently, I am not working on this project as I have focused my attention in an alternative implementation in Rust at https://github.com/clearlycloudy/e2r

# Dependencies

GLFW

OpenGL 3.3+

# Features supported:

shader based rendering

multithreaded work pool

concurrent pool and datastructures

math utility for graphics manipulation such as dual quaternions, projections, interpolators

.obj file, .md5anim, .md5mesh file loader

basic orientation manipulators

# Planed features:

custom memory management

statistics for memory and rendering functions

physically based renderer

# Video logs of progress:

https://youtu.be/pxU5Jtvp9Z4
