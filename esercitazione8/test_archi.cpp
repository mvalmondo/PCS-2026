#include "header_es6.hpp"
#include <iostream>
#include <cstdlib>



int main() { 
	undirected_edge<int> arco1(5,3);
	undirected_edge<int> arco2(1,2);
	undirected_edge<int> arco3(2,1);
	
	
	if (!(arco2==arco3)) {
		std::cerr<<"errore! (1,2) dovrebbe essere uguale a (2,1)!"<<"\n";
		return EXIT_FAILURE;
	}
	
	if (arco1.from()!=3 || arco1.to()!=5) {
		std::cerr<<"errore!! il costruttore dovrebbe mettere sempre il piu piccolo in from e il piu grande in to!"<<"\n";
		return EXIT_FAILURE;
	}
	
	if (!(arco2<arco1)) {
		std::cerr<<"errore! l'operatore < non ordina correttamente gli archi!"<< "\n";
		return EXIT_FAILURE;
	}
	
	std::cout<<"Test degli archi avvenuto con successso!"<<"\n";
	return EXIT_SUCCESS;
}


