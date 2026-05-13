#include "header_es6.hpp"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>

int main() {
	undirected_graph<int> g;
	g.add_edge(1,2);
	g.add_edge(1,3);
	g.add_edge(4,5);
	
	std::vector<int> n=g.neighbors(1);
	if (n.size()!= 2) { //i vicini di 1 ci aspettiamo essere il 2 e il 3
		return EXIT_FAILURE;
	}
	
	bool vicino2=false, vicino3=false;
	for(int v:n) {
		if (v==2){
			vicino2=true;
		}
		if (v==3) {
			vicino3=true;
		}
	}
	if (vicino2 &&vicino3) {
		std::cout<<"Test neighbors avvenuto con successo" << "\n";
		return EXIT_SUCCESS;
	}
	return EXIT_FAILURE;
}