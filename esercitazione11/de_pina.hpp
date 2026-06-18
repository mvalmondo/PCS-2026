#pragma once
#include "header_es9.hpp"
#include "finding_path.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <climits>



//inizializzo matrice S: crea una lista di k vettori ciascuno di lunghezza m (matrice kxm)
template <typename T>
std::vector<std::vector<bool>> inizializza_vettori_S(undirected_graph<T>& G, undirected_graph<T>& coalbero) {
	int m=G.all_edges().size(); //numero totale di archi in G
	int k=coalbero.all_edges().size(); //num archi del coalbero
	
	std::vector<std::vector<bool>> S(k, std::vector<bool>(m,false)); //inizializzo a 0 la matrice k x m
	
	int i=0;
	for (const auto& arco : coalbero.all_edges()) {
		int index_generale=G.edge_number(arco); //troviamo l'indice di questo arco nel grafo generale G
		if (index_generale!=-1) {
			S[i][index_generale]=true; //imposto a 1 la coordinata dell'arco del coalbero
		}
		i++;
	}
	return S;
}



//prodotto scalare, controlla se il ciclo minimo appena scoperto (C_i) è ortogonale o meno ai vettori di supporto successivi (S_j)
int prod_scalare(const std::vector<bool>& S, const std::vector<bool>& P) {
	int somma=0;
	for (size_t i=0; i<S.size(); i++) {
		if (S[i] && P[i]) { //se l'arco i-esimo è attivo sia in S che in P, incremento il contatore
			somma++;
		}
	}
	return somma%2; //restituisce il risultato modulo 2 (0 se pari, 1 se dispari)
}


//differenza simmetrica, viene usata subito dopo il controllo del prod_scalare; se il prodotto scalare tra il ciclo C_i e il vettore test successivo S_j è =1 deve aggiornare il vettore test facendo l'operazione di XOR; serve a manteneere l'indipendenza lineare dei vettori test per i passi successivi
std::vector<bool> differenza_simmetrica(const std::vector<bool>& S_j, const std::vector<bool>& S_i) {
	std::vector<bool> risultato(S_j.size());
	for (size_t x=0; x<S_j.size(); ++x) {
		risultato[x]= (S_j[x]!=S_i[x]); //l'operatore != tra booleani si comporta come lo XOR logico (true se diversi)
	}
	return risultato;
}





//1. duplico i nodi ottenendo v+ e v-
//2. per ogni archi (u,v) se l'arco è attivo allora si aggiunge l'arco coi nodi spaiati (di segno opposto) al G_primo altrimenti a G_primo si aggiunge l'arco coi segni concordi 
template <typename T> 
undirected_graph<T> costruisci_grafo_lifted(undirected_graph<T>& G, const std::vector<bool>& S_i) {
	undirected_graph<T> G_primo;
	int salto=G.all_nodes().size();
	int m=G.all_edges().size();
	for (int index=0; index<m;++index) {
		auto arco=G.edge_at(index);
		T u=arco.from();
		T v=arco.to();
		
		//controllo se l'arco corretne è attivo in S_i
		if (S_i[index]==true) {
			//se è attivo aggiunge a G_primo gli archi incrociati(u+, v-) e (u-, v+)
			G_primo.add_edge(u,v+salto);
			G_primo.add_edge(u+salto, v);
		} else { //se non è attivo, agginge a G_primo gli archi paralleli (con stesso segno: (u+,v+) e (u-,v-))
			G_primo.add_edge(u,v);
			G_primo.add_edge(u+salto,v+salto);
		}
	}
	return G_primo;
}



//3.per ogni vertice del grafo calcolo cammino minimo usando Dijkstra tra v- e v+ in G_primo
template <typename T>
std::vector<std::vector<T>> calcola_tutti_cammini(undirected_graph<T>& G, undirected_graph<T>& G_prime) {
    std::vector<std::vector<T>> tutti_i_cammini;
    int salto = G.all_nodes().size();
    
    for (const T& v : G.all_nodes()) {
        T sorgente = v + salto; // v-
        T destinazione = v;     // v+
        
        // Chiamiamo il tuo Dijkstra che genera il grafo albero
        undirected_graph<T> albero_dijkstra = dijkstra(G_prime, sorgente);
        
        // SICUREZZA: Sfruttiamo la tua funzione che estrae il cammino in modo perfetto
        std::vector<T> cammino = trova_cammino(albero_dijkstra, sorgente, destinazione);
        
        if (!cammino.empty()) {
            tutti_i_cammini.push_back(cammino);
        }
    }
    return tutti_i_cammini;
}



//4.costruisco vetttore di incidenza (C_mu)
//i cammini troati contengono i nodi sdoppiati di G_primo; per ogni cammino dobbiamo mappare gli attraversametni delgi originari incrementando modulo 2 (invertendo il booleano true/false) la posizione corrispondente; riportiamo i nodi sdoppiati ai valori reali appliando l'operatore modulo (%salto)
template <typename T>
std::vector<bool> costruisci_vettore_incidenza(undirected_graph<T>& G, const std::vector<T>& cammino, int salto) {
	int m=G.all_edges().size(); //lunghezza |E| 
	std::vector<bool> C_mu(m, false);
	if(cammino.size()<2) {
		return C_mu;
	}
	for (size_t i=0; i<cammino.size()-1; ++i) {
		T u_orig=cammino[i]%salto;
		T v_orig=cammino[i+1]%salto;
		
		if (u_orig!=v_orig) {
			T n1=std::min(u_orig, v_orig);
			T n2=std::max(u_orig,v_orig);
			undirected_edge<T> arco_originale(u_orig, v_orig);
			int index=G.edge_number(arco_originale);
			
			if (index!=-1) {
				//incrementare modulo 2 equivale a invertire lo stato del bit booleano
				C_mu[index]=!C_mu[index];
			}
		}
	}
	return C_mu;
}



//5. tra tutti i vettori d'incidenza generati, dobbiamo isolare e scartare quelli nulli e conservare l'unico ciclo avente il piu piccolo numero di 1 (cioe il minor numero di archi attivi)
template <typename T>
std::vector<bool> trova_ciclo_minimo(undirected_graph<T>& G, const std::vector<bool>& S_i) {
	undirected_graph<T> G_primo=costruisci_grafo_lifted(G, S_i); //genera il grafo ausiliario
	std::vector<std::vector<T>> cammini_trovati=calcola_tutti_cammini(G, G_primo); //estrae tutti i percorsi minimi tra le coppie di nodi speculari
	
	int m=G.all_edges().size();
	int salto=G.all_nodes().size();
	
	std::vector<bool> C_i(m,false); //inizializzo il vettore con tutti zeri (stradafando gli archi attivi diventeranno true)
	bool primo_ciclo_valido=true;
	int conteggio_minimo=0;
	
	for (const auto& cammino:cammini_trovati) {
		std::vector<bool> C_mu=costruisci_vettore_incidenza(G, cammino,salto); //traduco il percorso in vettore d'incidnza
		int numero_di_uno= std::count(C_mu.begin(),C_mu.end(), true); //conto il numero di 1 nel vettore
		if(numero_di_uno==0) { //se il ciclo è vuoto, con nessun arco attivo, vai avanti,ignoralo
			continue;
		}
		if (prod_scalare(C_mu, S_i)!=1) {
			continue;
		}
		if (primo_ciclo_valido) { //se è in assoluto il primo ciclo valido che analizza, il numero 1 diventa il punto di partenza
			C_i=C_mu;
			conteggio_minimo=numero_di_uno;
			primo_ciclo_valido=false; //dal prossimo ciclo faremo il confronto vero e proprio
		}
		else if (numero_di_uno < conteggio_minimo) { //per tutti i cicli successivi, usiamo il conteggio per vedere se batte il minimo attuale
			C_i=C_mu;
			conteggio_minimo=numero_di_uno; //aggiorna il numero minimo di uno
		}
		
	}
	if (std::count(C_i.begin(), C_i.end(), true) == 0) {
         for(size_t index = 0; index < S_i.size(); ++index) {
              if(S_i[index]) { 
                   C_i[index] = true; 
                   break; 
              }
         }
    }
	return C_i;
}














template <typename T>
std::vector<std::vector<bool>> de_pina(undirected_graph<T>& G, std::vector<std::vector<bool>>& matrice_S) {
	int k= matrice_S.size();
	std::vector<std::vector<bool>> B; //base dei cicli finale
	for (int i=0;i<k;++i) {
		std::vector<bool> C_i=trova_ciclo_minimo(G, matrice_S[i]);
		B.push_back(C_i);
		for (int j=i+1; j<k; ++j) {
			if (prod_scalare(C_i, matrice_S[j])==1) {
				matrice_S[j]=differenza_simmetrica(matrice_S[j], C_i);
			}
		}
	}
	return B;
}


