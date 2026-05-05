#include <iostream>
#include <vector>
#include <stdlib.h>
#include "is_sorted.hpp"
#include "alg_sort.hpp"
#include "randfiller.h"
#include "timecounter.h"
#include <algorithm>

int main() {
	randfiller rf;
	timecounter tc;
	std::vector<int> v;
	std::vector<std::string> s={"mela", "pera", "banana", "ciliegia"};
	
	double secs_v=0; 					//inizializzo il tempo del cronometro
	for (int i=0; i<100; i++) { 		//eseguo l'ordinamento 100 volte su dimensioni diverse
		int n= rand()% 8000+1; 			//dimensione random n tra 1 e 8000 
		v.resize(n); 					//ad ogni giro del for adatta il vecchio vettore alla nuova dimensione n
		rf.fill(v, -100,100); 			//riordino il vettore con valori casuali tra -100 e 100
		tc.tic();						//avvio il cronometro
		selection_sort(v);
		secs_v=secs_v + tc.toc();		//fermo il cronometro subito dopo aver usato l'algoritmo dell'ordinamento 
		if (!is_sorted(v)) {
			return EXIT_FAILURE;
		}
	}
	
	std::cout << "Selection Sort Test: PASSED" << "\n";
	std::cout << "Tempo impiegato per ordinare 100 vettori di dimensione random molto grandi: " << secs_v << "secondi" << "\n";
	tc.tic();
	selection_sort(s);
	double secs_s=tc.toc();
	
	if (is_sorted(s)) {
			std::cout << "SelectionSort test sulla stringa: PASSED" << "\n";
			std::cout << "Tempo impiegato per ordinare la stringa: " << secs_s << "secondi" << "\n";
			std::cout << "Stringa finale: " ;
			for (std::string y:s) std::cout <<y<< " ";
				std::cout <<"\n";
		}
		
		
	for (int n=4; n<=8192; n*=2 ) {
		std::vector<int> v1(n);
		rf.fill(v1,-1000,1000);
		std::vector<int> v2=v1; //v2 è la copia identita di v1 ma lo userò con sort
		tc.tic();
		selection_sort(v1);
		double tempo_alg_selection=tc.toc();
		
		tc.tic();
		std::sort(v2.begin(), v2.end());
		double tempo_sort=tc.toc();
		std::cout<< "Tempo impiegato dall'algoritmo selection: "<< tempo_alg_selection << "\n";
		std::cout<< "Tempo impiegato dal sort: "<< tempo_sort << "\n";
	
		
	}
	return EXIT_SUCCESS;
}
