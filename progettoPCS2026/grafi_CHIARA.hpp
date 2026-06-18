#pragma once
#include <iostream>
#include <map>
#include <set>
#include <iterator>
#include <algorithm>
#include <optional>

template<typename T>
class undirected_edge {
	T nodo1;
	T nodo2;

public:
	undirected_edge(const T& n1,const T& n2 ): 
	nodo1(n1),nodo2(n2){
		if (nodo1>nodo2){
			std::swap(nodo1,nodo2);
		}
	}
	T from() const {return nodo1; }
	T to() const {return nodo2; }
	
	bool operator<(const undirected_edge& other) const{
		if (nodo1!=other.nodo1){
			return nodo1<other.nodo1;
		}
		else{
			return nodo2<other.nodo2;
		}
	}
	
	bool operator==(const undirected_edge& other) const{
		return (nodo1==other.nodo1 && nodo2==other.nodo2);
	}
};
	
template <typename T>                     
std::ostream& 
operator <<(std::ostream& os, const undirected_edge<T>& e){
	os << "(" << e.from() << ","<< e.to() << ")";
	return os;
}


	
template <typename T>
class  undirected_graph {
	std::map<T, std::set<T>> adiacenze;               //mappa con come chiavi i nodi, come valori della chiave un set dei nodi vicini
                                                     //in questo modo ad ogni nodo ho già associato i suoi vicini, che non possono essere ripetuti in un set
public:
	undirected_graph() 
		:adiacenze() //costruttore default: crea grafo vuoto
	{}  
	undirected_graph(const undirected_graph&other) 
		: adiacenze(other.adiacenze)                 //costruttore di copia
	{}
	
	void add_edge(const T& u, const T& v){
		adiacenze[u].insert(v);
		adiacenze[v].insert(u);
	}
	
	std::set<T> neighbours(const T& s) const{
		if (adiacenze.count(s)==0){
			return std::set<T>();
		}
		return adiacenze.at(s);
	}
	
	std::set<T> all_nodes() const {
		std::set<T> elenco_nodi;
		for (const auto& coppia : adiacenze){
			elenco_nodi.insert(coppia.first);       //prendo solo il primo elemento della coppia (ovvero la chiave della mappa per ogni riga)
		}
		return elenco_nodi;
	}
	
	std::set<undirected_edge<T>> all_edges() const {
		std::set<undirected_edge<T>> elenco_archi;
		for (const auto& coppia : adiacenze){
			const T& u=coppia.first; 
			for (const T& v: coppia.second){
				if (u<=v){
					elenco_archi.insert(undirected_edge<T>(u,v));
				}
			}
		}
		return elenco_archi;
	}
	
	undirected_edge<T> edge_at(int indice) const{
		std::set <undirected_edge<T>> archi=all_edges();
		if (indice <0 || indice>=archi.size() ){
			std::exit(EXIT_FAILURE);
		}
		auto iteratore=archi.begin();
		std::advance(iteratore, indice);
		return *iteratore;   //restituisce cosa c'è dove punta l'iteratore
	}
	
	std::optional <int> edge_number(const undirected_edge<T>& e) const {
		std::set <undirected_edge<T>> archi=all_edges();
		auto iteratore=archi.find(e);
		if (iteratore==archi.end() ){
			return std::nullopt;
		}
		return std:: distance(archi.begin(),iteratore);
	}
	
	undirected_graph<T> operator-(const undirected_graph<T>& other) const {
		undirected_graph<T> risultato;
		std::set<T> nodi= all_nodes();
		for (const T& n : nodi){
			risultato.adiacenze[n];
		}
		auto archi_g1 =all_edges();
		auto archi_g2 = other.all_edges();
		for (const auto& arco: archi_g1){
			if (archi_g2.count(arco)==0){          //per ogni arco nel grafo g1, lo aggiungiamo al risultato se non compare nel grafo g2
				risultato.add_edge(arco.from(), arco.to());
			}
		}
		return risultato;
	}
};
	
	