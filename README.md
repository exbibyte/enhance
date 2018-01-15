# e2r

A very experimental/proof of concept/minimalistic 3D rendering engine in Rust language. It is very work in process and major work is in under way to factor and lay out engine components.

# Current implementations:

md5anim, md5mesh parser

obj parser

primite shape intersection routines

matrix utilities

bounding volume hierarchy

gjk intersection test

basic windowing

render backend using OpenGL ADS shader

# Todos:

Refactoring major components for engine to be localized

Hook up spatial acceleration structures prior to rendering

Implement trajectory controller

Implement model asset and texture management

Implement debugging and console input facilities

Implement multithreading support for front end and some of the back end pipelines

Implement tesselation and level of detail contoller

Implement shadows in render pipeline

Do feasibility study of using physically based rendering approach as an alternative rendering pipeline
