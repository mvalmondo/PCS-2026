#include <iostream>
#include <vector>
#include <stdlib.h>
#include "is_sorted.hpp"
#include "alg_sort.hpp"
#include "randfiller.h"
#include <algorithm>


//Testare gli algoritmi di ordinamento su 100 vettori di dimensioni scelte a caso
//Per ognuna delle dimensioni si testi gli algoritmi su vettori riempiti in modo casuale e che includano numeri sia negativi sia positivi

int main() {
	randfiller rf;
	std::vector<int> v;
	
//Si crei un vettore di std::string e lo si riempia con una decina di stringhe scelte a piacere, si verifichi quindi che gli algoritmi ordinano correttamente anche le stringhe 

	std::vector<std::string> s={"mela", "pera", "banana", "ciliegia", "frutta", "papaya", "mango", "kiwi"};
	
	for (int i=0; i<100; i++) {         //100 vettori di
		int n= rand()% 8000+1;          //dimensione random n tra 1 e 8000 
		v.resize(n);                    //ad ogni giro del for adatta il vecchio vettore alla nuova dimensione n
		rf.fill(v, -1000,1000);         //vettori riempiti in modo casuale che includano numeri sia positivi che negativi

		merge_sort(v, 0, v.size()-1); 
		
		if (!is_sorted(v)) {
			std::cout << "ERRORE: Il vettore NON è ordinato" << "\n";
			return EXIT_FAILURE;
		}
	}
	std::cout << "MergeSort test: PASSED" << "\n";
	
	//ora faccio test sulle stringhe
	merge_sort(s,0,s.size()-1);
	if (is_sorted(s)) {
			std::cout << "MergeSort test sulla stringa: PASSED" << "\n";
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
	
	
	