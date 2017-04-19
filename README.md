# e2

Minimalist 3D rendering and game engine done using a subset of C++ and C-style conventions.

Focus of the engine to provide low level control, os-like kernel and associated modules for component loading, independence from external libraries.

Only certain parts of C++ lanuage is used: templates, namespaces. Meanwhile other parts of C++ are avoided: implicit function invokations/copy constructors, minimal dependence on Standard Template Library. In conjunction, C-style functions, pointer instead of reference are used.
