#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>


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


// nuova versione dell'insertion_sort che serve per il quicksort_modificato (non gli passo semplicmetne il vec ma anche l'inizio e la fine del segmetno che l'insertionsort deve ordinare, prima che entri in azione il quicksort)
template<typename T>
void insertion_sort(std::vector<T>& vec, int sinistra, int destra) {
	for (int i = sinistra + 1; i <= destra; i++) {
		T value = vec[i];
		int j = i - 1;
		// Sposta gli elementi che sono più grandi di value verso destra
		while (j >= sinistra && vec[j] > value) {
			vec[j + 1] = vec[j];
			j = j - 1;
		}
		vec[j + 1] = value;
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



//IMPLEMENTAZIONE FUSION 
template<typename T> 
void fusion(std::vector<T>& vec, int sinistra, int centro, int destra) {
	//la funzione fusion crea due vettori temporanei Left e Right e ci copia dentro i valori delle due metà che deve unire
	int n1=centro-sinistra +1;
	int n2=destra-centro;
	std::vector<T> L(n1), R(n2); 
	for (int i=0; i<n1; i++) { //parte di sinistra del vettore
		L[i]=vec[sinistra+i];
	}
	for (int j=0; j<n2; j++) { //parte di destra del vettore
		R[j]=vec[centro+1+j];
	}
	
//la funzione prende un indice i della parte di sx e un indice j della parte di dx; guarda il primo elemento della parte Left e la prima di R; se la parte più piccola è la parte di sx, la mette nel vettore finale e sposta i in avanti; se la parte piu piccola è quella di dx, la mette nel vett finale e sposta j in avanti: 
	int i=0;
	int j=0;
	int k=sinistra;
	while(i<n1 && j < n2) {
		if (L[i]<= R[j]) {
			vec[k]=L[i];
			i=i+1;
		}
		else {
			vec[k]=R[j];
			j=j+1;
		}
		k=k+1;
	}
	
	while (i<n1) {
		vec[k]=L[i];
		i=i+1;
		k=k+1;
	}
	while (j<n2) {
		vec[k]=R[j];
		j=j+1;
		k=k+1;
	}
	
}

//IMPLEMENTAZIONE MERGESORT
template<typename T>
void merge_sort(std::vector<T>& vec, int sinistra, int destra) {

	if (sinistra<destra) {
		int centro= (sinistra+destra)/2;
		merge_sort(vec, sinistra, centro);
		merge_sort(vec, centro+1, destra);
		fusion(vec, sinistra, centro, destra);
	}
}



//IMPLEMENTAZIONE PARTITION
template<typename T> 
int partition(std::vector<T>& vec, int sinistra, int destra) {
	int p=sinistra+rand() % (destra-sinistra+1); //scelgo a caso il pivot
	if (p!=destra) {
		std::swap(vec[p], vec[destra]);
		p=destra;
	}
	int i=sinistra-1;
	for (int j=sinistra; j<=destra-1; j++) {
		if (vec[j]<=vec[p]) {
			i=i+1;
			std::swap(vec[i], vec[j]);
		}
	}
	std::swap(vec[i+1], vec[destra]);
	return i+1;
}



//IMPLEMENTAZIONE QUICKSORT
template<typename T>
void quick_sort(std::vector<T>& vec, int sinistra, int destra) {
	if (sinistra<destra) {
		int q=partition(vec, sinistra, destra);
		quick_sort(vec, sinistra, q-1);
		quick_sort(vec, q+1, destra);
	}
}



//IMPLEMENTAZIONE QUICKSORT_MODIFICATO
template<typename T>
void quicksort_modificato(std::vector<T>& vec, int sinistra, int destra) {
	//ciclo while per evitare la ricorsione su entrmabi i rami
	while (sinistra<destra) {
		//se il sottovettore ha dimensione<10, l'insertionsort è piu efficiente del quicksort
		if(destra-sinistra +1 <10){
			insertion_sort(vec, sinistra, destra);
			break; //interrompe il ciclo, l'ordinamento è fiinito per questo ramo 
		} else {
			int q= partition(vec, sinistra, destra); //q è il pivot
			//ordina sempre per prima la parte piu piccola del vettore per mantenere lo stack bilanciato
			if (q-sinistra<destra-q) {
				quicksort_modificato(vec, sinistra, q-1);
				sinistra=q+1;
			} else {
				quicksort_modificato(vec, q+1, destra);
				destra=q-1;
			}
		}
	}
}
	