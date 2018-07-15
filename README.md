### dirconf ###

dirconf is a simple C++ program whose propose is apply directories configuration files. Currently 
only implemented for nemo. For more 
information use `./dirconf --help` command.

### Build ###

Use the folowing commands to buil the CMake project.

    cmake .
    cmake --build .

#### Dependencies ####

In order to compile this software you have to use a C++ revision equal or highter to C++17 
(ISO/IEC 14882:2017). Also the following libraries are necessary:
- speed (https://github.com/killian1024/speed)
- boost 1.58
- libglib-2.0
- libgio-2.0
