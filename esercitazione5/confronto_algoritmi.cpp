#include <iostream>
#include <vector>
#include "is_sorted.hpp"
#include "alg_sort.hpp"
#include "randfiller.h"
#include "timecounter.h"
#include <algorithm>

int main() {
	randfiller rf;
	timecounter tc;
	
	
	for (int n=1; n<=200; n=n+4) {
		int n_vettori=100;
		
		//Si preallochino un centinaio di vettori della dimensione che si vuole testare 
		//questo crea un contenitore di 100 elementi dove ogni elemento è a sua volta un vettore di n interi
		std::vector<std::vector<int>> m(n_vettori, std::vector<int>(n));
		for(int i=0; i<n_vettori; i++) {
			rf.fill(m[i], -1000, 1000); //con la funzione randfiller riempio a caso ongi riga del contenitore
		}
		//creo 5 copie per non ordinare 5 volte lo stesso contenitore
		auto m_bubble=m;
		auto m_insertion=m;
		auto m_selection=m;
		auto m_merge=m;
		auto m_quick=m;
		auto m_sort=m;
		
		tc.tic();
		//Tramite un ciclo for, si ordinino tutti i vettori, misurando il tempo totale di esecuzione con delle tic()/toc() all’esterno del ciclo for
		for (int i=0; i<n_vettori; i++) {
			bubble_sort(m_bubble[i]);
		}
		double t_bubble=tc.toc();
		//Si calcoli la media, dividendo il tempo totale per il numero di vettori
		double media_bubble=t_bubble/n_vettori;
	
	
		tc.tic();
		for (int i=0; i<n_vettori; i++) {
			insertion_sort(m_insertion[i]);
		}
		double t_insertion=tc.toc();
		double media_insertion=t_insertion/n_vettori;
	
	
		tc.tic();
		for (int i=0; i<n_vettori; i++) {
			selection_sort(m_selection[i]);
		}
		double t_selection=tc.toc();
		double media_selection=t_selection/n_vettori;
	
	
		tc.tic();
		for (int i=0; i<n_vettori; i++) {
			merge_sort(m_merge[i],0,m_merge[i].size()-1);
		}
		double t_merge=tc.toc();
		double media_merge=t_merge/n_vettori;
		
	
		tc.tic();
		for (int i=0; i<n_vettori; i++) {
			quick_sort(m_quick[i],0,m_quick[i].size()-1);
		}
		double t_quick=tc.toc();
		double media_quick=t_quick/n_vettori;
		
		
		tc.tic();
		for (int i=0; i<n_vettori; i++) {
			std::sort(m_sort[i].begin(), m_sort[i].end());
		}
		double t_std_sort=tc.toc();
		double media_std_sort=t_std_sort/n_vettori;
		
		//std::cout<<"\n"<<"tempo del bubblesort: "<< t_bubble<<"\n"<<"tempo dell'insertionsort: "<<t_insertion<<"\n"<<"tempo del selectionsort: "<<t_selection<<"\n"<<"tempo del mergesort: "<<t_merge<<"\n"<<"tempo del quicksort: "<<t_quick<<"\n";
		//std::cout<<"\n"<<"dimensione: "<< n << "\n"<<"tempo medio del bubblesort: "<<media_bubble<<"\n"<<"tempo medio dell'insertionsort: "<<media_insertion<<"\n"<<"tempo medio del selectionsort: "<<media_selection<<"\n"<<"tempo medio del mergesort: "<<media_merge<<"\n"<<"tempo medio del quicksort: "<<media_quick<<"\n"<<"tempo medio del std_sort: "<<media_std_sort<<"\n";
		std::cout << n << " " << media_bubble << " " << media_insertion << " " << media_selection << " " << media_merge << " " << media_quick << " " << media_std_sort << std::endl;
	}
	
	
	return 0;
}
	
	
