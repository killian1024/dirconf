### dirconf ###

dirconf apply directories configuration files editing the GNOME Virtual file system directories 
metatada. JSON files are used to specify the configuration in the directories. Currently is only 
implemented for nemo. For more information about how write the configuration files and use the 
program use `dirconf --help` command.

### Build ###

Use the folowing commands to buil the CMake project.

    cmake .
    cmake --build .

#### Dependencies ####

In order to compile this software you have to use a C++ revision equal or highter to C++17 
(ISO/IEC 14882:2017). Also the following tools and libraries are necessary with a version equal or 
highter the specified one:
- GCC 8.1.0
- CMake 3.9
- speed (https://github.com/killian1024/speed)
- boost 1.58
- libglib 2.0
- libgio 2.0
