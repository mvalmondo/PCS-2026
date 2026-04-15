#include <iostream>
#include <vector>
#include <algorithm>


//implementazione BubbleSort

template<typename T>
void bubble_sort(std::vector<T>& vec) {
	int n=vec.size();
	
	for (int i=0; i<=n-2;i++) {
		for (int j=n-1; j>= i+1; j--) {    //j parte dalla fine del vettore (n-1) e scende verso l'inizio quindi verso la posizione i+1 (quindi la posizione j è >= della posizione di quello dopo!)
			if (vec[j]<vec[j-1]) {
				std::swap(vec[j], vec[j-1]);
			}
		}
	}
	

}

//implementazione InsertionSort

template<typename T>
void insertion_sort(std::vector<T>& vec) {
	int n=vec.size();
	
	for (int i=1; i<= n-1; i++) {       //prendo un elemento (il value) e lo faccio scorrere indietro finche non trova la sua posizione corretta nella parte gia ordinata del vettore
		T value=vec[i];
		int j=i-1;
		while (j>=0 && vec[j] > value) {
			std::swap(vec[j+1], vec[j]);
			j=j-1;
		}
		vec[j+1]=value;
	}
}

//implementazione SelectionSort
template<typename T>
void selection_sort(std::vector<T>& vec) {
	int n=vec.size();
	
	for (int i=0; i<= n-2; i++) {      //
		int min_index=i;
		for (int j=i+1; j<= n-1; j++) { 
			if (vec[j]<vec[min_index]) {
				min_index=j;
			}
		}
		if (min_index!=i) {
			std::swap(vec[i], vec[min_index]);
		}
	}
}
	