#pragma once
#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <queue>
#include <stack>
#include <map>
#include <climits>
#include <fstream>


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
	
	
	//metodo che di un nodo restituisce i vicini 
	std::vector<T> neighbors(const T& nodo) {
		std::vector<T> vicini;
		if (nodi.find(nodo)== nodi.end()) { //da cppreference uso find per trovare elementi con chiave specifica per dire che se il nodo non è nel grafo allora non avrà vicini (restituisce un vettore vuoto)
			return vicini;
		}
		for (const undirected_edge<T>& a:archi) { // ogni arco del contenitore è elemento della classe undirected_edge (che ha i metodi from() e to())
			if(a.from()==nodo) {
				vicini.push_back({a.to()}); //ricorda che push_back aggiunge elemento alla fine del vettore
			} else if (a.to()==nodo) {
				vicini.push_back({a.from()});
			}
		}
		return vicini;
		
	}
	
	
	//metodo che permette di aggiungere un arco al grafo
	void add_edge(const T& n1, const T& n2) {
		nodi.insert(n1); //inserisco i nodi nuovi nel set di nodi
		nodi.insert(n2);
		archi.insert(undirected_edge<T>(n1,n2)); //crea il collegamento con la classe undirected_edge e lo inserisce nel contenitore degli archi
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







//implemento il FIFO (first in first out)
template <typename T>
class fifo {
	private:
	std::queue<T> q;
	
	public:
	void put(const T& val) { //aggiunge val alla queue
		q.push(val); 
	}
	
	T get() { //per rimuovere il first 
		T elem_selezionato=q.front(); //l'elemento piu in testa viene tolto (first in first out)
		q.pop();
		return elem_selezionato;
	}
	
	bool empty() const {
		return q.empty();
	}
};


//implementazione del LIFO (Last in first out)
template <typename T>
class lifo {
	private:
	std::stack<T> l;
	
	public:
	void put(const T& val) { //aggiunge val allo stack
		l.push(val);
	}
	
	T get() { //per rimuovere il last
		T elem_selezionato=l.top(); //l'elemento piu in cima allo stack
		l.pop();
		return elem_selezionato;
	}
	
	bool empty() const {
		return l.empty();
	}
};




template <typename T, typename contenitore>
undirected_graph<T> graph_visit(undirected_graph<T>& G, const T& nodo_sorgente, contenitore& c) { //visita generica (BFS o DFS in base a se uno fifo o lifo), restituisce l'albero di visita
	
	undirected_graph<T> albero_visita; //grafo risultante è l'albero di visita
	std::set<T> nodi_visitati; //nodi già visitati
	
	c.put(nodo_sorgente); //salvo nel contenitore la sorgent
	nodi_visitati.insert(nodo_sorgente);
	while (!c.empty()) {
		T nodo_considerato =c.get(); 
		
		std::vector<T> lista_vicini=G.neighbors(nodo_considerato);
		for (auto iteratore=lista_vicini.begin(); iteratore!=lista_vicini.end(); iteratore++) {
			T vicino=*iteratore; //faccio scorrere l'iteratore su tutto il set dei vicini e prendo l'elemento dove punta l'iteratore
			
			if (nodi_visitati.find(vicino)==nodi_visitati.end()) {
				nodi_visitati.insert(vicino);
				c.put(vicino);
				albero_visita.add_edge(nodo_considerato, vicino);
			}
		} 
	}
	
	return albero_visita;
};



//per implementare la funzione dfs_recursive() 
template <typename T>
void dfs_ricorsiva_helper(undirected_graph<T>& G, const T& nodo_corrente, std::set<T>& nodi_visitati, undirected_graph<T>& albero_visita_ricorsiva) {
	nodi_visitati.insert(nodo_corrente); //marca il nodo attuale come visitato
	std::vector<T> lista_vicini=G.neighbors(nodo_corrente);
	for (auto iteratore =lista_vicini.begin(); iteratore!=lista_vicini.end(); iteratore++) {
		T vicino=*iteratore;
		if (nodi_visitati.find(vicino)==nodi_visitati.end()) {
			albero_visita_ricorsiva.add_edge(nodo_corrente, vicino);
			dfs_ricorsiva_helper(G, vicino, nodi_visitati, albero_visita_ricorsiva);
		}
	}
};

template <typename T>
undirected_graph<T> recursive_dfs(undirected_graph<T>& G, const T& nodo_sorgente) {
	std::set<T> visitati;
	undirected_graph<T> albero;
	dfs_ricorsiva_helper(G,nodo_sorgente, visitati, albero); //l'algoritmo cicla sui vicini del nodo attuale e per quelli non visitati crea un un sottoalbero
	return albero;
};



//implementazione dijkstra, per questa implementazione è stato utilizzato l'aiuto dell'IA 
template <typename T>
undirected_graph<T> dijkstra(undirected_graph<T>& G, const T& s) {
	undirected_graph<T> albero;
	std::map<T, int> dist; //array delle distanze
	std::map<T,T> pred; //array dei predecessori (uso map perche T è generico)
	std::set<T> nodi=G.all_nodes();
	for (const T& nodo:nodi) {
		dist[nodo]=INT_MAX;
		pred[nodo]=T(); //sentinella equivalente a -1
	}
	
	pred[s]=s;
	dist[s]=0;
	std::priority_queue<std::pair<int,T>, std::vector<std::pair<int,T>>, std::greater<std::pair<int,T>>> PQ; //coda di priorità (priority queue imostata come min-heap per estrarre la distanza minore; memorizza coppie del tipo <distanza, nodo>)
	for (const T& nodo:nodi) {
		PQ.push({dist[nodo],nodo});
	}
	while (!PQ.empty()) {
		auto [p,u] =PQ.top();
		PQ.pop();
		
		if (p>dist[u]) continue;
		for (const T& w:G.neighbors(u)) {  // foreach (w ∈ LA[u]) (non avendo specificato i pesi assumiamo peso=1)
			if (dist[w]>dist[u]+1) {
				dist[w]=dist[u]+1;
				pred[w]=u;
				PQ.push({dist[w],w});
			}
		}
	}
	
	for (const T& nodo:nodi) {
		if (pred[nodo] != T() && nodo!=s) {
			albero.add_edge(pred[nodo], nodo);
		}
	}
	return albero;
};



//utilizzo dell'IA per creare una funzione che che stampi il grafo in fomrato DOT, da utilizzare poi nel GraphViz per renderlo immagine 
template <typename T> 
void print_dot(undirected_graph<T>& G,const std::string& filename,const std::string& graph_name = "G") {
	std::ofstream file(filename);
	file << "graph " << graph_name << " {\n";
	for (const undirected_edge<T>& a : G.all_edges()) {
		file << "  " << a.from() << " -- " << a.to() << ";\n";
	}
	file << "}\n";
}









