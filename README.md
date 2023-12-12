# C++ Hackathon 2023

## Overview

This project is a C++ Parser which takes can data as arxml file and converts to yaml using tinyxml2 and yaml-cpp library.
## Prerequisites

Before you build and run this project, ensure you have the following installed:
- C++ Compiler (supporting C++11 standard)
- CMake (version 3.10 or higher)
- Git (for cloning the repository and fetching submodules)

## Building the Project

### Using CMake

1. **Clone the Repository:**
   ```bash
   git clone https://github.com/vamsisai9/cppHackathon_2023.git
   cd cPPHackathon

2.  **Create a Build Directory:**
    ```bash
    mkdir build && cd build

4.  **Generate Build System:**
    ```bash
     cmake ..
    
5. **Compile the Project:**
    ```bash
    cmake --build .
   
6.  **exe execution:**
    ```bash
    ./cPPHackathon.exe  <path to arxml>
    
