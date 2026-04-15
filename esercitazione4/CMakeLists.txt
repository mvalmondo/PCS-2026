cmake_minimum_required(VERSION 3.20)
project(esercitazione4)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wshadow -Werror=vla")


## Creo i target per gli eseguibili che effettueranno i test unitari
add_executable(test_bubblesort test_bubblesort.cpp timecounter.cpp)
add_test(NAME bubblesort COMMAND test_bubblesort)

add_executable(test_insertionsort test_insertionsort.cpp timecounter.cpp)
add_test(NAME insertionsort COMMAND test_insertionsort)

add_executable(test_selectionsort test_selectionsort.cpp timecounter.cpp)
add_test(NAME selectionsort COMMAND test_selectionsort)

enable_testing()
