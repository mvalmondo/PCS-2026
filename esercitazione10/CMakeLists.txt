cmake_minimum_required(VERSION 3.20)
project(esercitazione10)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wshadow -Werror=vla")

#passo la libreria eigen (suggerito da Gemini)
include_directories(/usr/include/eigen3)

add_executable(es10 gradiente_coniugato.cpp)