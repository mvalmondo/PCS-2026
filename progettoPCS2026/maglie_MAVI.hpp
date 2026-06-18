#pragma once
#include <vector>
#include <map>
#include <set>
#include "grafi_CHIARA.hpp" 
#include "netlist.hpp"
#include "adapter_REBECCA.hpp"



//primo metodo per trovare cicli nel grafo ovvero maglie del circuito; NON è detto che siano i minimi


//scrivo una funzione helper che legga il mio albero T ricavato tramite DFS ricorsiva. Voglio un cammino che parta dal nodo corrente v e arrivi al nodo destinazione u percorrendo nodi non ancora visitati. dopo la prima iterata il nodo corrente non è piu v, ma viene aggiornato
template <typename T> 
bool trova_cammino_helper(undirected_graph<T>& T_grafo, const T& nodo_partenza, const T& nodo_arrivo, std::set<T>& visitati, std::vector<T>& cammino) {
	
	visitati.insert(nodo_partenza);
	cammino.push_back(nodo_partenza);
	
	if (nodo_partenza==nodo_arrivo) {
		return true;
	}
	std::set<T> vicini_nodocorrente=T_grafo.neighbours(nodo_partenza); //il neighbors() di chiara restituiva un set
	
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
std::vector<std::vector<T>> trova_cammino(undirected_graph<T>& T_grafo, undirected_graph<T>& coalbero) {
	std::vector<std::vector<T>> cammino;
	for (const auto& arco: coalbero.all_edges()) {
		std::set<T> visitati;
		std::vector<T> step;
        int u = arco.from();
        int v = arco.to();
        trova_cammino_helper(T_grafo, v, u, visitati, step);
        step.push_back(v);
        cammino.push_back(step);
    }
	return cammino;
};




//metodo De Pina: questo è il secondo metodo per trovare le maglie; questo metodo trova i cicli minimi


//funzione per definire il grafo sdoppiato (PUNTO 1 e 2)
undirected_graph<int> costruisci_grafo_sdoppiato(const undirected_graph<int>& G, const std::vector<bool>& S_i ){
	undirected_graph<int> G_ausiliario;
	int salto= G.all_nodes().size();  //conto quanti nodi ha G-> sarà il mio salto tra il nodo positivo e il nodo corripsondente negativo
	std::set<undirected_edge<int>> archi_G= G.all_edges();
	for (auto& arco: archi_G){
		int u=arco.from();
		int v=arco.to();
		
		auto indice_arco=G.edge_number(arco);
		int i= indice_arco.value();
		if (S_i[i]==true){
			//creo linee tra nodi posituvi e negativi
			G_ausiliario.add_edge(u, v+salto);
			G_ausiliario.add_edge(u+salto,v);
		}
		else {
			//creo linee parallele, tra nodi dello stesso segno
			G_ausiliario.add_edge(u, v);
			G_ausiliario.add_edge(u+salto,v+salto);
		}
	}
	return G_ausiliario;
}


//(PUNTO 3)
std::vector<std::vector<int>> trova_cammini_minimi(const undirected_graph<int>& G,const undirected_graph<int>& G_ausiliario){
	std::vector<std::vector<int>> cammini_minimi;
	int salto= G.all_nodes().size();
	
	for (int v: G.all_nodes()){
		std::map<int, int> distanze;                                 
		std:: map<int, std::optional<int>> predecessori;
		dijkstra_CHIARA(G_ausiliario, v+salto, distanze, predecessori); //il dijsktra parte da v-, ovvero da v+salto
		
		//ora estraiamo i nodi dai predecessori e salviamoli nel vettore percorso
		std::vector<int> percorso;
		if(distanze[v]!=std::numeric_limits<int>::max()){
			int nodo_corrente=v;
			while (predecessori[nodo_corrente].has_value() && nodo_corrente!=v+salto){ //se il nodo_corrente ha un predecessore con un valore numerico e il nodo_corrente è diverso dal v- allora aggiungi il nodo_corente al percorso di predecessori trovati con il dijkstra
				percorso.push_back(nodo_corrente);
				nodo_corrente=predecessori[nodo_corrente].value();
			}
			percorso.push_back(nodo_corrente);//aggiungo la destinazione(v-) al percorso e lo chiudo
		}
		cammini_minimi.push_back(percorso);//aggiungo il percorso ai miei cammini minimi-> avrò un vettore di cammini minimi
	}
	return cammini_minimi;
}

//(PUNTO 4)
std::vector<bool> costruisci_vettore_incidenza(const undirected_graph<int>&G, const std::vector<int>& percorso, const int&salto){
	int dimensione=G.all_edges().size();
	std::vector<bool> vettore_incidenza;
	for (int i=0; i<dimensione; i++){
		vettore_incidenza.push_back(false); //inizializziamo il vettore con tutti false ovvero con tutti 0 (archi non accesi) 
	}
	for (int i=0; i<(int)percorso.size()-1; i++){
		int u=percorso[i];
		int v=percorso[i+1];
		int u_corretto=u % salto;
		int v_corretto=v % salto;
		if (u_corretto==0) {
			u_corretto=salto;
		}
		if (v_corretto==0) {
			v_corretto=salto;
		}
		
		undirected_edge<int> arco(u_corretto,v_corretto);
		if (G.edge_number(arco).has_value()) { //test per vedere se c'è un valore da estrarre-> in caso di optional darebbe errore
            int indice=G.edge_number(arco).value();
			vettore_incidenza[indice] = !vettore_incidenza[indice] ; //se l'arco ha un valore, cioe esiste allora è acceso, cioe è 1, true 
        }
	}
	return vettore_incidenza;
}

//PUNTO 5(di tutti i cammini minimi trova quello con vettore di incidenza con minor numero di 1)
std::vector<bool> trova_ciclo_minimo(const undirected_graph<int>& G, std::vector<bool>& S_i){
	undirected_graph<int> G_ausiliario= costruisci_grafo_sdoppiato(G, S_i);
	std::vector<std::vector<int>> cammini_minimi=trova_cammini_minimi(G,G_ausiliario);
	
	std::vector<bool> ciclo_minimo;
	
	int salto=G.all_nodes().size();
	int minimo_uno=std::numeric_limits<int>::max();
	
	for (const auto& i: cammini_minimi){
		std::vector<bool> vettore_incidenza=costruisci_vettore_incidenza(G, i, salto);
		
		int conteggio_uno=std::count(vettore_incidenza.begin(), vettore_incidenza.end(), true);
	
	
		if(conteggio_uno>0 && conteggio_uno<minimo_uno){
			ciclo_minimo=vettore_incidenza;
			minimo_uno=conteggio_uno; //aggiorno il minimo e il vettore di incidenza se ne trovo uno con meno 1
		}
	}
	return ciclo_minimo; //resttuisce il vettore di incidenza del ciclo minimo
}

//operazioni per implementare lo pseudocodice finale di DePina
int prodotto_scalare(const std::vector<bool>& A, const std::vector<bool>& B ){
	int somma=0;
	for (int i=0; i<(int)A.size(); i++){
		somma= somma+A[i]*B[i];
	}
	return somma%2;
}

std::vector<bool> differenza_simmetrica(const std::vector<bool>& A, const std::vector<bool>& B ){
	std::vector<bool> risultato;
	for (int i=0; i<(int)A.size(); i++){//definisco con costa compare i, ovvero un altro inetro-> per evitare warning;
		risultato.push_back(A[i]!=B[i]);//risultato di XOR darà vero o falso-> lo aggiungiamo al vettore dei risultati
	}
	return risultato;
}

//DE PINA
std::vector<std::vector<bool>> de_pina(const undirected_graph<int>& G, const undirected_graph<int>& coalbero){
	std::vector<std::vector<bool>> matrice_S_i;
	for (int i=0; i<(int)coalbero.all_edges().size(); i++){//inizializzo i vettori S_i
		std::vector<bool> S_i;
		for (int j=0; j<(int)G.all_edges().size(); j++){
			S_i.push_back(false);
		}
		matrice_S_i.push_back(S_i);            
	}	
	int indice=0;
	for (const auto& arco: coalbero.all_edges()){
		int posizione= G.edge_number(arco).value();//posizione dell'arco in G-> sarà la posizione del vettore colonna S_i dove sarà=true
		matrice_S_i[indice][posizione]= true;
		indice++;//incremento indice-> scorro su tuttu gli archi del coalbero mentre scorro sulla matrice S_i
	}
	std::vector<std::vector<bool>> B;
	for (int i=0; i<(int)coalbero.all_edges().size(); i++){
		std::vector<bool> Ci= trova_ciclo_minimo(G, matrice_S_i[i]);
		B.push_back(Ci);
		for (int j=i+1; j<(int)coalbero.all_edges().size(); j++){
			if (prodotto_scalare(Ci, matrice_S_i[j])==1){
				matrice_S_i[j]=differenza_simmetrica(matrice_S_i[j], matrice_S_i[i]);
			}
		}
		
	}
	return B;
}





//scrivo una funzione che mi trasformi il vettore di booleani del depina in vettore di nodi (sequenza di nodi, cosi che possa poi sfruttarlo nel costruisci_B e nella costruzione del sistema lineare)
std::vector<int> converti_depina_helper(const std::vector<bool> cicli_bool, const undirected_graph<int>& G) {
	//raccolto gli archi in archi_ciclo
	std::vector<undirected_edge<int>> archi_ciclo;
	int i=0;
	for (const auto& arco:G.all_edges()) {
		if (cicli_bool[i]==true) { 
			archi_ciclo.push_back(arco);
		}
		i=i+1;
	}
	//costruisco nodi seguendo la catena di archi
	std::vector<int> nodi;
	nodi.push_back(archi_ciclo[0].from());
	nodi.push_back(archi_ciclo[0].to());
	
	//aggiungo nodi seguendo la catena di archi
	while (nodi.size() < archi_ciclo.size()) {
		int ultimo=nodi.back();
		for (const auto& arco:archi_ciclo) {
			int u=arco.from();
			int v=arco.to();
			
			if (u==ultimo && v!= nodi[nodi.size()-2]) {
				nodi.push_back(v);
				break;
			} else if (v==ultimo && u!= nodi[nodi.size()-2]) {
				nodi.push_back(u);
				break;
			}
		}
	}
	nodi.push_back(nodi[0]); //chiudo il ciclo tornando al nodo iniziale
	return nodi;
}



//convertitore dei ciclo del depina in sequenza di nodi
std::vector<std::vector<int>> convertitore_depina(const std::vector<std::vector<bool>>& cicli_bool, const undirected_graph<int>& G) {
	std::vector<std::vector<int>> cicli_nodi; 
	for (const auto& singolo_ciclo : cicli_bool) {
		cicli_nodi.push_back(converti_depina_helper(singolo_ciclo, G));
	}
	return cicli_nodi;
}