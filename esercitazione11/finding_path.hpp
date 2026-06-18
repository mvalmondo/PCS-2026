#pragma once
#include "header_es9.hpp"
#include <iostream>
#include <vector>
#include <set>



template <typename T> 
bool trova_cammino_helper(undirected_graph<T>& T_grafo, const T& nodo_partenza, const T& nodo_arrivo, std::set<T>& visitati, std::vector<T>& cammino) {
	
	visitati.insert(nodo_partenza);
	cammino.push_back(nodo_partenza);
	
	if (nodo_partenza==nodo_arrivo) {
		return true;
	}
	std::vector<T> vicini_nodocorrente=T_grafo.neighbors(nodo_partenza); //il mio neighbors() restituiva un vector
	
	for (const auto& vicino : vicini_nodocorrente ) {
		if (!visitati.count(vicino)) {
			if (trova_cammino_helper(T_grafo, vicino, nodo_arrivo, visitati, cammino)) {
				return true;
			}
		}
	}
	cammino.pop_back();
	return false;
};




template <typename T>
std::vector<T> trova_cammino(undirected_graph<T>& T_grafo, const T& nodo_partenza, const T& nodo_arrivo) {
	std::set<T> visitati;
	std::vector<T> cammino;
	
	trova_cammino_helper(T_grafo, nodo_partenza, nodo_arrivo, visitati, cammino);
	return cammino;
}

