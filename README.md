# Matrix Display Application

This project is a Qt 6 application developed in C++ that simulates a fixed-grid LED matrix. It can render text with optional scrolling or display a digital clock. The project is built using CMake and includes Doxygen-ready comments for generating API documentation.

## Features

- Fixed $100 \times 12$ virtual LED grid to mimic a physical display.
- Text mode with optional horizontal scrolling for long strings.
- Clock mode with blinking separator.
- Runtime pixel color selection.
- Doxygen-ready API comments.

## Project Structure

- [doc_doxygen](doc_doxygen)
- [headers/mainwindow.h](headers/mainwindow.h)
- [headers/matrixdisplay.h](headers/matrixdisplay.h)
- [headers/matrixfont.h](headers/matrixfont.h)
- [src/main.cpp](src/main.cpp)
- [src/mainwindow.cpp](src/mainwindow.cpp)
- [src/matrixdisplay.cpp](src/matrixdisplay.cpp)
- [CMakeLists.txt](CMakeLists.txt)
- [Doxyfile](Doxyfile)

## Dependencies Installation

```sh
sudo apt install qt6-base-dev qt6-base-dev-tools qt6-l10n-tools
sudo apt install cmake ninja-build build-essential
sudo apt install doxygen graphviz
```

## Build Instructions

```sh
mkdir -p build
cd build
cmake ..
cmake --build .
```

## Build Clean

```sh
cd build
make clean
```

## Run

```sh
./Matrix_Display
```

## Shortcut

|Keys|Description|
|----|-----------|
|Ctrl + H| Hide the control Widget|

## Documentation

Doxygen documentation are available in the doc_doxygen folder.
You can generate the documentation with the Project Configuration using this command:

```sh
#if you are in build/ folder
cd .. 
doxygen Doxyfile
```
