cmake_minimum_required(VERSION 3.13.0) # only tested on cmake 3.26.4
project(Phos)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Phos")

add_subdirectory("src/Phosboot" "Phosboot")
add_subdirectory("src/Phoskrnl" "Phoskrnl")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "")

add_subdirectory("src/Phosdb" "Phosdb")