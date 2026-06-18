#include "finding_path.hpp"
#include "header_es9.hpp"
#include "de_pina.hpp"
#include <iostream>




int main() {
	undirected_graph<int> G;
	G.add_edge(1,3);
	G.add_edge(6,3);
	G.add_edge(6,8);
	G.add_edge(6,1);
	G.add_edge(1,4);
	G.add_edge(4,6);
	G.add_edge(6,7);
	G.add_edge(8,9);
	G.add_edge(7,6);
	G.add_edge(4,7);
	G.add_edge(4,2);
	G.add_edge(1,2);
	G.add_edge(7,2);
	G.add_edge(5,7);
	G.add_edge(5,2);
	G.add_edge(7,9);
	
	undirected_graph<int> T=recursive_dfs(G, 1);
	undirected_graph<int> C=G-T; //coalbero
	
	//cicli con DFS

    std::cout << "=== CICLI FONDAMENTALI (metodo DFS) ===" << std::endl;
    for (const auto& arco : C.all_edges()) {
        int u = arco.from();
        int v = arco.to();
        std::cout << "\nTrovato arco del coalbero: (" << u << ", " << v << ")" << std::endl;
        std::cout << "Cerco il cammino tra " << u << " e " << v << " nell'albero T..." << std::endl;

        std::vector<int> cammino = trova_cammino(T, u, v);

        if (!cammino.empty()) {
            std::cout << "-> Ciclo trovato: ";
            for (int nodo : cammino) std::cout << nodo << " ";
            std::cout << "-> (" << u << ", " << v << ")" << std::endl;
        } else {
            std::cout << "-> Errore: cammino non trovato nell'albero." << std::endl;
        }
    }
    
    
    //test de pina
    std::cout << "\n=== BASE DI CICLI MINIMI (De Pina) ===" << std::endl;

    std::vector<std::vector<bool>> matrice_S = inizializza_vettori_S(G, C);

    std::cout << "Numero archi G: " << G.all_edges().size() << std::endl;
    std::cout << "Numero archi coalbero (k): " << C.all_edges().size() << std::endl;

    std::vector<std::vector<bool>> base = de_pina(G, matrice_S);

    std::cout << "\nBase di cicli trovata (" << base.size() << " cicli):" << std::endl;
    for (int i = 0; i < (int)base.size(); i++) {
        std::cout << "C[" << i << "]: archi attivi -> ";
        bool ciclo_vuoto = true;
        for (int j = 0; j < (int)base[i].size(); j++) {
            if (base[i][j]) {
                auto arco = G.edge_at(j);
                std::cout << "(" << arco.from() << "," << arco.to() << ") ";
                ciclo_vuoto = false;
            }
        }
        if (ciclo_vuoto) std::cout << "[ciclo vuoto - possibile errore]";
        std::cout << std::endl;
    }

    return 0;
}