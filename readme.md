# MultiLibrary

A C++ library that covers multiple low level systems.

## Info

This library requires premake 5 to create the build projects/makefiles. Execute premake within the "projects" folder. You must always specify at least one option which is the action to perform, whether it be "clean", the toolchain to use for compiling (for example, gmake), etc. There are also optional flags and options like --static-runtime (self explanatory) and --os=os_name (currently, MultiLibrary supports Windows, Linux and Mac OSX). I have to warn you right now, even though it *SHOULD* support Mac OSX, it isn't tested. Step it up Apple.

All thirdparty libraries and header files can be placed inside projects/os_name/thirdparty (or a path of your choice if you change --thirdparty-folder) and follows the structure:

    include (has the header files)
    lib (has the files for linking)
        x64 (x86-64 linking files)
        x86 (x86 linking files)
    src (has the files for source code, right now only glew can be compiled)
