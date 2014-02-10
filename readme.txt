This library requires premake 4 (no less than 4.4) to create the build projects/makefiles.
Execute premake within the "Projects" folder.
You must always specify at least one option which is the action to perform, whether it be "clean", the toolchain to use for compiling (for example, gmake), etc.
There are also optional flags and options like --static-runtime (self explanatory) and --os=os_name (currently, MultiLibrary supports Windows, Linux and Mac OSX).