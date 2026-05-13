#include "header_es6.hpp"
#include <iostream>
#include <cstdlib>
#include <algorithm>


//test funzioni (verificare che il set ordini in modo lessicografico i nodi e archi)
int main() {
	undirected_graph<int> g;
	g.add_edge(1,2);
	if (g.all_edges().size() != 1) { //l'arco (1,2) è stato inserito?
		std::cerr<<"errore! numero di archi errato" <<"\n";
		return EXIT_FAILURE;
	}
	std::cout<<"Test add_edge avvenuto con successo!"<<"\n";
	return EXIT_SUCCESS;
}
