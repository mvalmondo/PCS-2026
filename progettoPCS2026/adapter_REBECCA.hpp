#pragma once 
#include <optional>
#include <map>
#include <set>
#include <list>
#include <stdexcept>
#include <vector>
#include <ostream>
#include <algorithm>
#include <fstream>
#include <queue>
#include <stack>
#include <limits>
#include "grafi_CHIARA.hpp"

// Contenitore con politica FIFO (coda) --> usato per BFS
template<typename T>
class fifo { // First In - First Out
	std::queue<T> q;
public:
	void put(T val) {  
		q.push(val); // inserisco un elemento in fondo alla coda
	}
	T get() {
		T v=q.front(); // prendo un elemento che si trova in cima alla coda
		q.pop(); // lo rimuovo dalla struttura
		return v; // restituisco valore preso e rimosso
	}
	bool empty() const { // valore booleano per dire se la coda è vuota o meno
		return q.empty();
	}
};

// Contenitore con politica LIFO (stack) --> usato per DFS
template<typename T> 
class lifo { // Last In - First Out
	std::stack<T> s;
public:
	void put(T val) {
		s.push(val); // inserisco un elemento in cima alla pila
	}
	T get() {
		T v=s.top(); // prendo l'elemento che si trova in cima alla pila
		s.pop(); // lo rimuovo
		return v; // lo restituisco 
	}
	bool empty() const { // la pila è vuota?
		return s.empty();
	}
};

template<typename T, typename Container>
undirected_graph<T> graph_visit(const undirected_graph<T>& g, T sorgente, Container contenitore) {
// ricevo un grafo g per riferimento costante, per non copiarlo e non modificarlo
// nodo sorgente da cui far partire la visita
// il tipo di contenitore che passo come argomento stabilirà se la visita si comporterà come
// BFS usando la coda FIFO  --> esplora il vicinato			   oppure come
// DFS usando lo stack LIFO --> profondità
	
	undirected_graph<T> grafo; 
	// grafo di output inizialmente vuoto, man mano che scopriamo nuovi nodi, aggiungiamo archi
	std::set<T> visitati; 
	// insieme set che tiene traccia dei nodi già scoperti
	
	contenitore.put(sorgente);
	visitati.insert(sorgente);
	// inizializzo la visita e inserisco il nodo di partenza dentro il contenitore marcandolo come già "visitato"
	
	while (!contenitore.empty()) { // finchè ci sono odi da visitare ...
		T corrente = contenitore.get();
		// estraggo il prossimo nodo da visitare
		// se il contenitore è FIFO --> prenderà il più vecchio (BFS)
		// se il contenitore è LIFO --> prenderà l'ultimo inserito (DFS)
		
		auto vicini_opt = g.neighbours(corrente);
		if(!vicini_opt) continue;
		
		// copio i vicini in un vettore e li ordino per renderli indipendenti dall'ordine di inserimento degli archi nel file 
		std::vector<T> vicini_ordinati(vicini_opt->begin(),vicini_opt->end());
		std::sort(vicini_ordinati.begin(),vicini_ordinati.end());
		
		for (T vicino: vicini_ordinati) {
			if(!visitati.count(vicino)) { 
			//controllo che il vicino in esame non sia già stato visitato
			//count restituisce 1 se l'elemento esiste nel set, 0 altrimenti
				visitati.insert(vicino);
				contenitore.put(vicino);
				grafo.add_edge(corrente,vicino);
			}
		}
	}
	
	return grafo;
}

// === funzione helper per la DFS ricorsiva ===
template<typename T>
void helper_recursive_dfs(const undirected_graph<T>& g, T corrente, std::set<T>& visitati, undirected_graph<T>& albero) {
	visitati.insert(corrente);
	
	auto vicini_puliti = g.neighbours(corrente);
	std::vector<T> vicini_ordinati(vicini_puliti.begin(), vicini_puliti.end());
	std::sort(vicini_ordinati.begin(),vicini_ordinati.end());
	
	for (T vicino : vicini_ordinati) {
		if (!visitati.count(vicino)) {
			albero.add_edge(corrente,vicino);
			helper_recursive_dfs(g,vicino,visitati,albero);
		}
	}
}

// === funzione ===
template<typename T>
undirected_graph<T> recursive_dfs(const undirected_graph<T>& g, T sorgente) {
	undirected_graph<T> albero;
	std::set<T> visitati;
	helper_recursive_dfs(g, sorgente, visitati, albero);
	return albero;
}

// === dijkstra ===
template<typename T>
undirected_graph<T> dijkstra(const undirected_graph<T>& g, T sorgente) {
	
	// grafo che conterrà l'albero dei cammini minimi risultatnte
	undirected_graph<T> albero_cammini_min;
	
	// inizializzazione delle distanze
	const double INF=std::numeric_limits<double>::infinity(); // definisco una costante che rappresenta il valore matematico di infinito
	//alternativamente pongo un valore arbitrariamente grande --> const double INF=99999999.0 
	std::map<T, double> dist; 
	// creo una mappa per associare a ogni nodo la sua distanza provvisoria dalla sorgente
	for (T nodo : g.all_nodes()) {
		dist[nodo] = INF; //inizalizzo la distanza di tutti i nodi a infinito --> nodo non ancora raggiungibile
	}
	dist[sorgente]=0.0; //distanza del nodo sorgente da se stesso
	
	// mappa che tiene traccia del predecessore ottimale di ogni nodo
	std::map<T,T> predecessore;
	
	// per farlo funzionare in modo efficiente esamino prima i nodi più vicini: coda a priorità
	using coppia = std::pair<double,T>;
	using min_queue = std::priority_queue<coppia, std::vector<coppia>, std::greater<coppia>>;
	min_queue pq;
	pq.push({0.0, sorgente}); //primo elemento inserito nella coda: nodo sorgente a distanza zero
	
	std::set<T> finalizzati; // tengo traccia dei nodi per cui abbiamo già trovato il cammino minimo
	
	while (!pq.empty()) {
		auto[d,u] =pq.top();
		// estraggo la coppia in cima alla coda: u = nodo corrente; d = distanza 
		pq.pop();
		
		if(finalizzati.count(u)) {
			continue;
		}
		finalizzati.insert(u);
		
		// se il nodo corrente ha un predecessore nella mappa, significa che 
		// l'arco che lo collega al cammini minimo è definitivo.
		// --> lo inseriamo nell'albero
		if (predecessore.count(u)) {
			albero_cammini_min.add_edge(predecessore[u],u);
		}
				
		auto vicini_opt = g.neighbours(u);
		if(!vicini_opt) continue;
		
		// ordino i vicini
		std::vector<T> vicini_ordinati(vicini_opt->begin(),vicini_opt->end());
		std::sort(vicini_ordinati.begin(),vicini_ordinati.end());
		
		for(T v: vicini_ordinati) {
			// fissdimso il peso di ogni arco a 1.0
			double peso=1.0;

			double nuova_dist = dist[u] + peso; 
			if (nuova_dist < dist[v]) {
				dist[v] = nuova_dist;
				predecessore[v]=u;
				pq.push({nuova_dist,v});
			}
		}
	}
	
	return albero_cammini_min;
}


// === funzione per esportazione grafo in formato DOT per Graphviz
template<typename T>
void esporta_dot(const undirected_graph<T>& g, const std::string& nome_file, const std::string& titolo="G") {
	std::ofstream f(nome_file);
	f << "graph " << titolo << " {\n";
	for (const auto& arco: g.all_edges()) {
		f << " \"" << arco.from() << "\" -- \"" << arco.to() << "\";\n";
	}
	f << "}\n";
}




















template <typename T>
void dijkstra_CHIARA(const undirected_graph<T>& grafo, 
			  const T& sorgente,
			  std::map<T, int>& distanze,                                 //uso una mappa che ad ogni nodi di tipo T associ un intero,ovvero la distanza con la sorgente
			  std:: map<T, std::optional<T>>& predecessori){                //stessa cosa, ma associa ad ogni nodo il suo predecessore
	         
	auto nodi= grafo.all_nodes();
	for (const auto& nodo: nodi){                                  //inizializzo distanze e predecessori
		distanze[nodo]=std::numeric_limits<int>::max();
		predecessori[nodo]= std:: nullopt;         //per gli interi non posso avere numeri negativi-> uso std::optional e nullopt per dire che non esiste un predecessore
	}
	predecessori[sorgente]=sorgente;
	distanze[sorgente]=0;
	
	std::set<T> coda_priorità;
	for (const auto& n:nodi){
		coda_priorità.insert(n);
	}
	while(!coda_priorità.empty()){
		auto it_minimo=coda_priorità.begin();
		for (auto it=coda_priorità.begin(); it!=coda_priorità.end(); it++){                 //troviamo il nodo con distanza minima; a questo punterà l'iteratore it_minimo 
			if (distanze[*it]<distanze[*it_minimo]){
				it_minimo=it;
			}
		}
		T u = *it_minimo;          //l'iteratore it_minimo punta al nodo u
		coda_priorità.erase(it_minimo);            //nel set cancello cosa sta nella posizione dell'iteratore minimo; corrisponde a dequeue
		
		
		if (distanze[u]== std::numeric_limits<int>::max()){
			break; //caso speciale: la distanza minima è infinito-> tutti i nodi distano infinito dalla sorgente 
		}
		for (const auto& v: grafo.neighbours(u)){
			if (coda_priorità.count(v)){
				int peso_arco =1;  				//metto il peso dell'arco =1
				if (distanze[v]>distanze[u] + peso_arco){
					distanze[v]=distanze[u] + peso_arco;
					predecessori[v]=u;
				}
			}                                                                                          
		}
	}
}


