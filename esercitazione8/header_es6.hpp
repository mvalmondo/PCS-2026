#include <iostream>
#include <algorithm>
#include <vector>
#include <set>

template<typename T>
//COSA è un arco? funzione che definisce l'undirected_edge:
class undirected_edge {
	private:
	T nodo1;
	T nodo2;
	
	public:
	undirected_edge(T n1, T n2) : nodo1(std::min(n1,n2)), nodo2(std::max(n1,n2))
	{} 
	T from() const {return nodo1; } //il costruttore mi garantisce che il nodo1 sia il valore minore
	T to() const { return nodo2; }
	
	
	bool operator<(const undirected_edge& other_arco) const { //questo operatore deve confrontare gli archi e stabilire un ordine
		if (nodo1 != other_arco.nodo1) {
			return nodo1 <other_arco.nodo1;
		} else {
			return nodo2<other_arco.nodo2;
		}
	}
	
	
	bool operator==(const undirected_edge& other_arco) const { //questo operatore riconosce quando un arco è uguale ad un altro [avendo forzato il costruttore a mettere come nodo1 il piu piccolo e il nodo2 come il piu grande, questo operatore riconosce quando il nodo1 di un arco è uguale al nodo1 di un altro arco e stessa cosa per il nodo2]
		return nodo1==other_arco.nodo1 && nodo2==other_arco.nodo2;
	}
};


//l'operatore<< è l'operatore che mi stampa il grafo in modo leggibile (va fuori dalla classe perche prende os che non è un oggetto della classe)
template<typename T>
std::ostream& operator<<(std::ostream& os, const undirected_edge<T>& arco) {
	os<<"("<<arco.from()<< ", "<<arco.to()<<")";
	return os;
}




template<typename T>
class undirected_graph {
	private: 
	std::set<T> nodi; //il set garantisce l'unicita e l'ordine dei nodi
	std::set<undirected_edge<T>> archi; //set di archi

	public:
	
	//costruttore di default
	undirected_graph() { 
		//inizializzo un grafo vuoto (lo fa automaticamente avendo usato contenitori di STL)
	}
	
	
	//costruttore di copia
	undirected_graph(const undirected_graph& other) {
		nodi=other.nodi;
		archi=other.archi;
	}
	
	
	//metodo che di un nodo restituisce i vicini (restituisce un vettore)
	std::vector<T> neighbors(const T& nodo) {
		std::vector<T> vicini;
		if (nodi.find(nodo)== nodi.end()) { //da cppreference uso find per trovare elementi con chiave specifica per dire che se il nodo non è nel grafo allora non avrà vicini (restituisce un vettore vuoto)
			return vicini;
		}
		for (const undirected_edge<T>& a:archi) { // ogni arco del contenitore è elemento della classe undirected_edge (che ha i metodi from() e to())
			if(a.from()==nodo) {
				vicini.push_back(a.to()); //ricorda che push_back aggiunge elemento alla fine del vettore
			}
		}
		return vicini;
		
	}
	
	
	//metodo che permette di aggiungere un arco al grafo
	void add_edge(const T& n1, const T& n2) {
		nodi.insert(n1); //inserisco i nodi nuovi nel set di nodi
		nodi.insert(n2);
		archi.insert(undirected_edge<T>(n1,n2)); //crea il collegamento con la classe undirected_edge e lo inserisce nel contenitore (in coda) degli archi
	}
	
	
	//metdodo che restituisce tutti gli archi
	std::set<undirected_edge<T>> all_edges() {
		return archi;
	}
	
	
	//metodo che restituisce tutti i nodi
	std::set<T> all_nodes() {
		return nodi;
	}
	
	
	//metodo che dato un arco, ne restituisce la sua numerazione all’interno del grafo
	int edge_number(const undirected_edge<T>& arco_cercato) {
		int i=0;
		for (auto it=archi.begin(); it!=archi.end(); ++it) { //essendo in un set uso un iteratore per scorrere gli elementi
			if (*it == arco_cercato) {
				return i;
			}
			i=i+1;
		}
		return -1; //se l'arco non esiste nel grafo mi dà -1 
	}
	
	
	//metodo che dato un numero d’arco, restituisce il corrispondente oggetto arco all’interno del grafo
	undirected_edge<T> edge_at(int index) {
		if (index>= archi.size()) { //controllo che l'indice richiesto non sia troppo grande, ma che esiste nel grafo
			std::cerr << "L'indice dell'arco non è valido!" << "\n";
			return undirected_edge<T>(T(),T());
		}
		auto it = archi.begin(); //puntatore posizionato sul primo elemento del set
		for (size_t i=0; i<index; i++) { //scorro l'iteratore nel set
			++it;
		}
		return *it; //prendo l'oggetto puntato dal puntatore 
	}
	
	
};



//operatore che permette di calcolare la differenza tra due grafi [fare G1-G2 vuol dire trovare un GrafoRisultato che contiene solo cio che c'è in G1 ma non in G2]


//osservazione: io non ho ADD_NODE() ma solo add_edge() che implicitamente aggiunge nodi (se esiste un arco, esso avrà i suoi nodi) 



template <typename T>
undirected_graph<T> operator-(undirected_graph<T>& G1, undirected_graph<T>& G2) {
	undirected_graph<T> G;
	
	std::set<undirected_edge<T>> archi_G1=G1.all_edges(); //recupero tutti gli archi del pirmo grafo 
	std::set<undirected_edge<T>> archi_G2=G2.all_edges(); //recupero gli archi del secondo grafo
	
	for (const undirected_edge<T>& arco:archi_G1) { //se l'arco è solo in G1 allora lo aggiungo al G finale
		if (archi_G2.find(arco)==archi_G2.end()) {
			G.add_edge(arco.from(), arco.to());
		}
	}
	return G;
};
