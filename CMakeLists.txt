cmake_minimum_required(VERSION 3.28)
project(MandelbrotSet)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(MandelbrotSet main.cpp
        Complex.h
        includes.h
        MandelbrotSet.cpp
        MandelbrotSet.h
        App.cpp
        App.h
)

target_link_libraries(MandelbrotSet
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
)

