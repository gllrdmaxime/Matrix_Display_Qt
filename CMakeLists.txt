cmake_minimum_required(VERSION 3.16)

project(Matrix_Display VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/matrixdisplay.cpp
    headers/mainwindow.h
    headers/matrixdisplay.h
    headers/matrixfont.h
)

qt_add_executable(Matrix_Display
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
)

target_include_directories(Matrix_Display PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(Matrix_Display PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(Matrix_Display PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Matrix_Display
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

qt_finalize_executable(Matrix_Display)