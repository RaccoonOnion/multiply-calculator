# This is the cmake file

# Compile flags constants
SET(CMAKE_CXX_FLAGS  "--std=c++20")
SET(CMAKE_EXE_LINKER_FLAGS  "-lmpfr -lgmp -lgmpxx")

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

cmake_minimum_required(VERSION 3.16)

project(calculator)

add_executable(calculator source.cpp assign.cpp compute.cpp check.cpp)


