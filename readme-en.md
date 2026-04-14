# scaffolding.ixx

## Project Introduction

This is a project scaffolding template using C++23, including a simple example to help you get started quickly. The project leverages modern C++ features and uses CMake for building and testing.

## Quick Start

1. Create a repository from this template
2. Introduce f-project series dependencies
   Place the dependent modules in 3rdparty (or introduce them via git submodule) and register them in the root CMakeLists.txt

## Project Structure

- `3rdparty`: Third-party libraries.
- `modules`: Project modules, including source code and test files.
    - `src/`: Source code directory, containing main C++ source files and module declaration files.
    - `CMakeLists.txt`: CMake configuration file for building the project.

## Building the Project

It is recommended to use CLion + MSVC for project development (currently only MSVC can directly use modules).

1. Open CLion and select `File -> Open`, then select the `scaffolding.ixx` directory.

2. Configure the MSVC toolchain in CLion:
    - Select `File -> Settings`.
    - Navigate to `Build, Execution, Deployment -> Toolchains`.
    - Click the `+` button to add a new toolchain and select `Visual Studio`.
    - Enter the installation directory of your local `Visual Studio`.

3. Configure CMake:
    - Select `File -> Settings`.
    - Navigate to `Build, Execution, Deployment -> CMake`.
    - Change the `Generator` to `Let CMake decide`.
    - Click `Reset Cache and Reload CMake Project`

4. Load the project and build:
    - Select `Build -> Build Project` to build the project.
