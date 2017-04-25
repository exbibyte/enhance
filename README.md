# Enhance/e2

Minimalist 3D rendering and game engine developed in C++.

Focus of the engine to provide low level control, customizable kernel, loadable game modules, independence from external libraries.

Only certain parts of C++ is leveraged such as templates, namespaces. Meanwhile other parts of C++ are avoided: implicit function invokations/copy constructors having expensive operations, minimal dependence on Standard Template Library.

# Current Progress

The engine is being revised in its implementation of core modules and multithreading facilities. Major parts of modules are being rewritten from the first attempt to provided as simply as possible, which currently reside in branch e2.

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