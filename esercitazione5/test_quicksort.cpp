#include <iostream>
#include <vector>
#include <stdlib.h>
#include "is_sorted.hpp"
#include "alg_sort.hpp"
#include "randfiller.h"
#include <algorithm>

int main() {
	randfiller rf;
	
//Testare gli algoritmi di ordinamento su 100 vettori di dimensioni scelte a caso
//Per ognuna delle dimensioni si testi gli algoritmi su vettori riempiti in modo casuale e che includano numeri sia negativi sia positivi
	std::vector<int> v;
	std::vector<std::string> s={"casa", "collina", "albero", "mattone", "tetto", "cortile", "stanza"};
	
	for (int i=0; i<100; i++) {         //100 vettori di
		int n= rand()% 8000+1;          //dimensione random n tra 1 e 8000 
		v.resize(n);                    //ad ogni giro del for adatta il vecchio vettore alla nuova dimensione n
		rf.fill(v, -1000,1000);         //vettori riempiti in modo casuale che includano numeri sia positivi che negativi 
		
		quicksort_modificato(v, 0, v.size()-1);
		
		if (!is_sorted(v)) {
			std::cout << "ERRORE: Il vettore NON è ordinato" << "\n";
			return EXIT_FAILURE;
		}
	}
	std::cout << "QuickSort test: PASSED" << "\n";	


//ora testiamo il quicksort sulle stringhe
	quicksort_modificato(s,0, s.size()-1);
	
	if (is_sorted(s)) {
			std::cout << "Quicksort test sulla stringa: PASSED" << "\n";
			std::cout << "Stringa finale: " ;
			for (const std::string& y:s) {
				std::cout <<y<< " ";
			}
			std::cout <<"\n";
	} else {
		std::cout << "ERRORE:LE stringhe non sono ordinate!"<< "\n";
	}
	return EXIT_SUCCESS;
}