# memory-analyzer
## Introduction

This project was primarily used as a thesis subject so it's real world uses are questinable at best.
If you still want to use this tool then below are the ways to do so.

## Prerequisites

- The tool was made and tested in Ubuntu 24.04.02, but it should work on any Linux machine
Installed libraries and tools:
- clang
- gtkmm 4
- CMake
- ninja
- cloned llvm-project (https://github.com/llvm/llvm-project)

## Compile

After cloning the llvm-project, go into the clang-tools-extra directory and clone this project.
Edit the CMakeLists.txt in the same directory to add the following line: `add_subdirectory(memory-analyzer)`

It is prefered to make a directory for the build as well. You can do this anywhere you would like,
for the rest of the document this will be refered to as BUILD_DIR.

In the BUILD_DIR execute the following command: `cmake -G Ninja PATH_TO_THE_LLVM_PROJECT/llvm -DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra"`

This will generate the build.ninja file.

After this you only have to run the `ninja memory-analyzer` command, which will compile the tool and the llvm-project.

There can be compilation errors connected to the -fno-rtti and the -fno-exceptions compile flags in the above mentioned ninja file.
These occure because the llvm-project uses these flags and during the compilation attach these to the ninja file, 
but the gtkmm 4 library isn't compatible with them.

To fix this, simply open the ninja fila in BUILD_DIR and remove those 2 flags from the following files: `control/Main.cpp`, `view/MainWindow.cpp`.

After this the compilation should be successfull.

## Usage

The usable executable will be place in BUILD_DIR/bin with the name memory-analyzer. Simply running it will give you the option to browse for a .cpp file to compile and analyze.

IMPORTANT!

The tool will not detect basic compile errors or warnings, if your file cannot be normally compiled then the tool will not give you the generic cpp errors and warnings as a normal compiler would, instead it will output "Compilation failed". The single cpp file should also be compilation ready on it's own. No other file should be required, as this will also generate the same output.

The analyzing happens statically, so the given cpp file isn't run, which can result to a number of false positives. To determine whether the warnings given are accurate or not are the responsibility of the user.