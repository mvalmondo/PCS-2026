#include "header_es6.hpp"
#include <iostream>
#include <cstdlib>

int main() {
	undirected_graph<int> g;
	g.add_edge(10,20);
	g.add_edge(30,40);
	
	auto archi=g.all_edges();
	auto it=archi.begin();
	it++;
	undirected_edge<int> secondo_arco=*it;
	
	if (g.edge_number(secondo_arco)!=1) {
		return EXIT_FAILURE;
	}
	if (!(g.edge_at(1)==secondo_arco)) {
		return EXIT_FAILURE;
	}
	std::cout<<"Test edge_number e edge_at avvenuti con successo!"<<"\n";
	return EXIT_SUCCESS;
}


