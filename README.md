# Matrix Display Application

This project is a Qt 6 application designed to display a matrix, which can render a clock or text. The application is built using C++ 17 and utilizes the Qt framework for its graphical user interface.

## Project Structure

- **CMakeLists.txt**: Configured to compile the application with the necessary Qt components.
- **main.cpp**: The entry point of the application, initializing the Qt framework and the main window.
- **mainwindow.h**: Header file for the MainWindow class.
- **mainwindow.cpp**: Implementation of the MainWindow class, managing UI interactions and rendering.
- **mainwindow.ui**: Qt Designer file for the main window's user interface.
- **matrix.h**: Header file for the Matrix class, which models the display grid.
- **matrix.cpp**: Implementation of the Matrix class.
- **clock.h**: Header file for the Clock class, responsible for time-keeping logic.
- **clock.cpp**: Implementation of the Clock class.
- **textrenderer.h**: Header file for the TextRenderer class.
- **textrenderer.cpp**: Implementation of the TextRenderer class for displaying text on the matrix.
- **README.md**: Documentation for the project.

## Setup Instructions

1. Ensure you have CMake and Qt 6 installed on your system.
2. Clone the repository or download the project files.
3. Navigate to the project directory:
   ```
   cd matrix_display
   ```
4. Create a build directory and navigate into it:
   ```
   mkdir build
   cd build
   ```
5. Run CMake to configure the project:
   ```
   cmake ..
   ```
6. Build the project:
   ```
   cmake --build .
   ```
7. Run the application:
   ```
   ./Matrix_Display
   ```