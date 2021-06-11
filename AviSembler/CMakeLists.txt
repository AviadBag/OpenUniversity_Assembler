# CMakeList.txt : CMake project for AviSembler, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
project(AviSembler C)

include_directories(include)
file(GLOB SOURCES "src/*.c")

# Add source to this project's executable.
add_executable (AviSembler ${SOURCES})