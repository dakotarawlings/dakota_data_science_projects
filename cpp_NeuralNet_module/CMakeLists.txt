cmake_minimum_required(VERSION 3.4...3.18)
project(test LANGUAGES CXX)

include_directories(C:/Users/dakot/AppData/Local/Programs/Python/Python310/Include)
include_directories(C:/Users/dakot/AppData/Local/Programs/Python/Python310/lib/site-packages/pybind11/include)
include_directories(C:/Users/dakot/AppData/Local/Programs/Python/Python310/lib/site-packages/pybind11)
include_directories(C:/Users/dakot/AppData/Local/Programs/Python/Python310/libs)

list(APPEND CMAKE_PREFIX_PATH C:/Users/dakot/AppData/Local/Programs/Python/Python310/lib/site-packages/pybind11/share/cmake/pybind11)
list(APPEND CMAKE_PREFIX_PATH C:/Users/dakot/AppData/Local/Programs/Python/Python310/lib/site-packages/pybind11)

find_package(Python COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 CONFIG REQUIRED)


pybind11_add_module(FFNN_pymodule FFNN_module.cpp)