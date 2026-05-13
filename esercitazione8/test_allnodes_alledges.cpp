#include "header_es6.hpp"
#include <iostream>
#include <cstdlib>
#include <set>


int main() {
	undirected_graph<int> g;
	g.add_edge(1, 2);
	g.add_edge(2, 3);
	g.add_edge(10, 20);
	std::set<int> nodi = g.all_nodes();
	if (nodi.size() != 5) { //mi aspetto che il numero di nodi sia 5
		std::cerr << "errore! Il numero di nodi è errato! Trovati: " << nodi.size() << "\n";
		return EXIT_FAILURE;
	}
	auto archi = g.all_edges();
	if (archi.size() != 3) {
		std::cerr << "errore! Il numero di archi è errato!" << "\n";
		return EXIT_FAILURE;
	}
	undirected_edge<int> arco_cercato(1, 2);
	if (archi.find(arco_cercato) == archi.end()) {
		std::cerr << "Errore: arco (1,2) non trovato nel set degli archi!" << "\n";
		return EXIT_FAILURE;
	}
	std::cout << "Test all_nodes e all_edges è avvenuto con successo" << "\n";
	return EXIT_SUCCESS;
}