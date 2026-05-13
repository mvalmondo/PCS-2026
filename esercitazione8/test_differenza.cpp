#include "header_es6.hpp"
#include <iostream>
#include <cstdlib>

int main() {
	undirected_graph<int> g1,g2;
	g1.add_edge(1,2);
	g1.add_edge(2,3);
	g2.add_edge(1,2);
	
	undirected_graph<int> g=g1-g2;
	if (g.all_edges().size()==1 && g.edge_at(0).from()==2) {
		std::cout<<"test operatore differenza avvenuto con successo!"<<"\n";
		return EXIT_SUCCESS;
	}
	return EXIT_FAILURE;
}