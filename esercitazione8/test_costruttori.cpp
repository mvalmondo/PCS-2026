#include "header_es6.hpp"
#include <iostream>
#include <cstdlib>



int main() {
	undirected_graph<int> g_orig;
	if (g_orig.all_nodes().size() != 0 || g_orig.all_edges().size() != 0) {
		return EXIT_FAILURE;
	}
	g_orig.add_edge(1, 2);
	g_orig.add_edge(2, 3);
	
	undirected_graph<int> g_copia(g_orig); //crea la copia del grafo
	if (g_copia.all_edges().size() != g_orig.all_edges().size()) {
		std::cerr << "errore! La copia non ha lo stesso numero di archi" << "\n";
		return EXIT_FAILURE;
	}
	
	g_orig.add_edge(4,5);
	if (g_copia.all_edges().size() == g_orig.all_edges().size()) {
		std::cerr << "errore! Modificando l'originale non si dovrebbe modificare anche la copia!" << "\n";
		return EXIT_FAILURE;
	}
	std::cout<<"Test dei costruttori avvenuto con successo"<<"\n";
	return EXIT_SUCCESS;
}
