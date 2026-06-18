#include <eigen3/Eigen/Dense>
#include <iostream>
#include <vector>
#include <iomanip>
#include "netlist.hpp"
#include "circuito.hpp"
#include "adapter_REBECCA.hpp"
#include "maglie_MAVI.hpp"
#include "sistema_lineare.hpp"



int main(int argc, char* argv[]) {
	std::string nome_file= "netlist.txt";
	if (argc>1) {
		nome_file=argv[1];
	} else {
		std::cout<<"Nessun file specificato.Uso il file di default: "<<nome_file<< "\n";
	}
    std::vector<ramo> netlist = leggi_netlist(nome_file);

    undirected_graph<int> G;
    std::map<undirected_edge<int>, ramo> info_arco;
    costruisci_grafo(netlist, G, info_arco);

    std::cout << "Nodi: ";
    for (int n : G.all_nodes()) {
	    std::cout << n << " ";
	}
	std::cout << "\n";


    std::cout << "Archi:\n";
    for (const auto& [arco, r] : info_arco)
        std::cout << arco << " -> " << r.tipologia << " = " << r.valore << "\n";

    // spanning tree e coalbero
    int sorgente = *G.all_nodes().begin();
    undirected_graph<int> T = recursive_dfs(G, sorgente);
    std::cout << "\n=== VERIFICA ALBERO T ===\n";
    for (const auto& arco : T.all_edges()) {
	    std::cout << "Arco in T: " << arco.from() << " -- " << arco.to() << "\n";
	}
	std::cout << "\n=== VERIFICA CO-ALBERO ===\n";
	undirected_graph<int> coalbero = G - T;
	for (const auto& arco : coalbero.all_edges()) {
		std::cout << "Arco in Co-Albero: " << arco.from() << " -- " << arco.to() << "\n";
		}

    std::vector<std::vector<int>> cammini = trova_cammino(T, coalbero);
	std::set<undirected_edge<int>> archi_coalbero = coalbero.all_edges();
	auto it_arco = archi_coalbero.begin();

	for (size_t i = 0; i < cammini.size(); ++i) {
    // Recuperiamo l'arco del co-albero corrispondente a questo cammino
    	int u = it_arco->from();
    	int v = it_arco->to();
    
    	std::cout << "Maglia " << i + 1 << " (generata da arco del co-albero (" << u << " -> " << v << ")): ";
    
    // stampiamo il percorso di nodi nell'albero T che unisce u e v
    // per chiudere la maglia (il ciclo), l'arco (u,v) del co-albero stesso fa parte del ciclo
    	for (int nodo : cammini[i]) {
        	std::cout << nodo << " ";
    	}
    	std::cout << "\n";
    
    	std::advance(it_arco, 1); // Passa all'arco successivo del co-albero
	}
    
    std::vector<std::vector<bool>> base= de_pina(G,coalbero); //ciclo base di de pina (è un vettore di 0 e 1 (boooleani))
	std::vector<std::vector<int>> cicli_depina = convertitore_depina(base, G); //converte il vettore degli archi attivi (base) in vettore con i nodi

	// stampa di verifica cicli De Pina convertiti in nodi
	std::cout << "\n=== CICLI DE PINA (sequenze di nodi) ===\n";
	for (int i = 0; i < (int)base.size(); i++){
		std::cout << "Ciclo " << i+1<< ": ";
        for (int j = 0; j < (int)base[i].size(); j++){
            std::cout << base[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "\n";
    
    
	for (int i = 0; i < (int)cicli_depina.size(); i++) {
    	std::cout << "Ciclo " << i+1 << ": ";
    	for (int nodo : cicli_depina[i]) {
        	std::cout << nodo << " ";
    	}
    	std::cout << "\n";
	}
	
	
	auto B = costruisci_B(cicli_depina, G, info_arco);
	auto R = costruisci_R(cicli_depina, G, info_arco);
	auto v = costruisci_v(cicli_depina, G, info_arco);

	std::vector<std::vector<double>> B_pina = costruisci_B(cammini, G, info_arco);
    std::vector<std::vector<double>> R_pina = costruisci_R(cammini, G, info_arco);
    std::vector<double> v_pina = costruisci_v(cammini, G, info_arco);

	std::cout << std::endl;

	std::cout << "==================\n";
	std::cout << "Stampa per il DFS\n";
	std::cout << "==================\n";
	// stampa di verifica B
	std::cout << "\nMatrice B (" << B.size() << " x " << B[0].size() << "):\n";
	for (const auto& riga : B) {
    	for (double val : riga) {
        	std::cout << std::setw(4) << val << " ";
    	}
    	std::cout << "\n";
	}

    // stampa di verifica R
	std::cout << "\nMatrice R (" << R.size() << " x " << R[0].size() << "):\n";
	for (const auto& riga : R) {
    	for (double val : riga) {
        	std::cout << std::setw(4) << val << " ";
    	}
    	std::cout << "\n";
	}

    // stampa dei termini noti
    std::cout<<"\nVettore termini noti v (dimensione " << v.size() << "):\n";
    for (double val : v) {
        std::cout << "[" << std::setw(4) << val << "]\n";
    }

    Eigen::VectorXd i_maglia = calcola_correnti(B,R,v);
    std::cout << "\n[TEST] Correnti di maglia calcolate:\n" << i_maglia << "\n";
    calcola_tensioni_resistori(B, R, i_maglia, G, info_arco);

	std::cout << std::endl;

	std::cout << "==================\n";
	std::cout << "Stampa per il DePina\n";
	std::cout << "==================\n";

	std::cout << "\nMatrice B_pina (" << B_pina.size() << " x " << B_pina[0].size() << "):\n";
	for (const auto& riga : B_pina) {
    	for (double val : riga) {
        	std::cout << std::setw(4) << val << " ";
    	}
    	std::cout << "\n";
	}

	std::cout << "\nMatrice R_pina (" << R_pina.size() << " x " << R_pina[0].size() << "):\n";
	for (const auto& riga : R_pina) {
    	for (double val : riga) {
        	std::cout << std::setw(4) << val << " ";
    	}
    	std::cout << "\n";
	}

    std::cout<<"\nVettore termini noti v_pina (dimensione " << v_pina.size() << "):\n";
    for (double val : v_pina) {
        std::cout << "[" << std::setw(4) << val << "]\n";
    }

    Eigen::VectorXd i_maglia_pina = calcola_correnti(B_pina,R_pina,v_pina);
    std::cout << "\n[TEST] Correnti di maglia calcolate:\n" << i_maglia_pina << "\n";
    calcola_tensioni_resistori(B_pina, R_pina, i_maglia_pina, G, info_arco);

	return 0;
}