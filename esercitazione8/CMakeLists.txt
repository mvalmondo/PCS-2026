cmake_minimum_required(VERSION 3.20)
project(esercitazione8)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wshadow -Werror=vla")


## Creo i target per gli eseguibili che effettueranno i test unitari
add_executable(test_archi test_archi.cpp)
add_test(NAME archi COMMAND test_archi)

add_executable(test_costruttori test_costruttori.cpp)
add_test(NAME costruttori COMMAND test_costruttori)

add_executable(test_neighbors test_neighbors.cpp)
add_test(NAME neighbors COMMAND test_neighbors)

add_executable(test_allnodes_alledges test_allnodes_alledges.cpp)
add_test(NAME allnodes_alledges COMMAND test_allnodes_alledges)

add_executable(test_addedge test_addedge.cpp)
add_test(NAME addedge COMMAND test_addedge)

add_executable(test_edgenumber_edgeat test_edgenumber_edgeat.cpp)
add_test(NAME edgenumber_edgeat COMMAND test_edgenumber_edgeat)

add_executable(test_differenza test_differenza.cpp)
add_test(NAME differenza COMMAND test_differenza)


enable_testing()
