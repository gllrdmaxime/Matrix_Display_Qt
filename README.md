# Matrix Display Application

This project is a Qt 6 application that simulates a fixed-grid LED matrix capable of rendering text or a digital clock.

## Features

- Fixed $100 \times 10$ virtual LED grid to mimic a physical display.
- Text mode with optional horizontal scrolling for long strings.
- Clock mode with blinking separator.
- Runtime pixel color selection.
- Doxygen-ready API comments.

## Project Structure

- [CMakeLists.txt](CMakeLists.txt)
- [headers/mainwindow.h](headers/mainwindow.h)
- [headers/matrixdisplay.h](headers/matrixdisplay.h)
- [headers/matrixfont.h](headers/matrixfont.h)
- [src/main.cpp](src/main.cpp)
- [src/mainwindow.cpp](src/mainwindow.cpp)
- [src/matrixdisplay.cpp](src/matrixdisplay.cpp)

## Build Instructions

```sh
mkdir -p build
cd build
cmake ..
cmake --build .
```

## Run

```sh
./Matrix_Display
```

## Documentation

Doxygen comments are available. Generate the documentation with your preferred Doxygen configuration, for example:

```sh
doxygen -g Doxyfile
doxygen Doxyfile
```