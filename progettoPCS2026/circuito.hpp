#pragma once
#include <map>
#include "netlist.hpp"
#include "grafi_CHIARA.hpp"


//scriviamo la funzione che trasforma il circuito (ora fatto di rami, che arrivano dalla funzione netlist) in grafo: la funzione prende in input (oltre che il ramo proveniente dalla netlist) anche il grafo G(che in grafi_CHIARA è una map) e la mappa di informazioni relative all'arco (R/V, valore, nodo_partenza e nodo_arrivo)

void costruisci_grafo (const std::vector<ramo>& netlist, 
                       undirected_graph<int>& G, 
                       std::map<undirected_edge<int>, ramo>& info_arco) {
	for (const ramo& r : netlist) {
		G.add_edge(r.nodo_partenza, r.nodo_arrivo);
		undirected_edge<int> arco(r.nodo_partenza, r.nodo_arrivo);
		info_arco[arco]=r;
	}
	
}