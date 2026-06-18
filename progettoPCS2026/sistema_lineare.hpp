#pragma once
#include <eigen3/Eigen/Dense>
#include <map>
#include <vector>
#include <iostream>
#include "netlist.hpp"
#include "maglie_MAVI.hpp"
#include "grafi_CHIARA.hpp"
#include "gradiente_coniugato_MAVI.hpp"

std::vector<std::vector<double>> costruisci_B(const std::vector<std::vector<int>>& cicli, const undirected_graph<int>& G, const std::map<undirected_edge<int>, ramo>& info_arco) {
	std::vector <undirected_edge<int>> resistori;
	//faccio un ciclo for per estrarre i resistori per ogni arco
	for (const auto& arco:G.all_edges()) {
		if (info_arco.count(arco) && info_arco.at(arco).tipologia[0]=='R') {
			resistori.push_back(arco);
		}
	}
	int m = resistori.size(); 
	int n= cicli.size();
	
	//inizializzazione della matrice B a zero
	std::vector<std::vector<double>> B(m, std::vector<double>(n,0.0));
	
	for (int j=0; j<n; j++) {
		const std::vector<int>& ciclo = cicli[j];
		
		for (int k=0; k<(int)ciclo.size()-1; k++){
			int u=ciclo[k];
			int v=ciclo[k+1];
			
			undirected_edge<int> arco(u,v);
			//cerchiamo l'arco appena ricostruito tra i resistori
			for (int i=0; i<m; i++) {
				if (resistori[i]==arco) {
					//se la maglia percorre l'arco da min a max assengo +1, altrimenti (da max a min) assegno -1
					if (u<v){
						B[i][j]=+1.0;
					}
					else {
						B[i][j]= -1.0;
					}
					break;
				}
			} 
		}
	}
	return B;
}

std::vector<std::vector<double>> costruisci_R(const std::vector<std::vector<int>>& cicli, const undirected_graph<int>& G, const std::map<undirected_edge<int>, ramo>& info_arco) {
	std::vector<double> resistori;
	//faccio un ciclo for per estrarre i resistori per ogni arco
	for (const auto& arco:G.all_edges()) {
		if (info_arco.count(arco) && info_arco.at(arco).tipologia[0]=='R') {
			resistori.push_back(info_arco.at(arco).valore);
		}
	}

	int m = resistori.size(); 
	std::vector<std::vector<double>> R(m,std::vector<double>(m,0.0));

	for (int j=0; j<m; j++) {
		R[j][j]=resistori[j];
	}

	return R;
}

// v=termine noto 
std::vector<double> costruisci_v(const std::vector<std::vector<int>>& cicli, const undirected_graph<int>& G, const std::map<undirected_edge<int>, ramo>& info_arco) {
	int n_maglie = cicli.size(); //
	std::vector<double> generatori(n_maglie,0.0); //
	undirected_edge<int> arco_iniziale(0,0);
	std::vector<undirected_edge<int>> archi_generatori(n_maglie,arco_iniziale);

	int i=0;
	for (const auto& arco:G.all_edges()) {
		if (info_arco.count(arco) && info_arco.at(arco).tipologia[0]=='V') {
			archi_generatori[i]=arco;
			i++;
		}
	}

	int n_gen = archi_generatori.size();

	for (int j=0; j<n_maglie; j++) {
		const std::vector<int>& ciclo = cicli[j]; // mi salvo il cammino
		for (int k=0; k<(int)ciclo.size()-1; k++) {
			int u=ciclo[k];
			int v=ciclo[k+1];
			undirected_edge<int> arco(u,v);
			for (int i=0; i<n_gen; i++) {
				if (archi_generatori[i]==arco) {
					if (u<v) {
						generatori[j]+=info_arco.at(arco).valore; 
					} else {
						generatori[j]-=info_arco.at(arco).valore;
					}
				
					break;
				}
			} 
		}
	}
	return generatori;
}

Eigen::VectorXd calcola_correnti(std::vector<std::vector<double>> B,std::vector<std::vector<double>> R,std::vector<double> v) {
	const double tol=1.0e-15;
	const int it_max=100000;
	unsigned int it=0;

	int righe_B = B.size();
	int colonne_B = B[0].size();
	int righe_R = R.size();
	int colonne_R = R[0].size();
	int dimensione_v = v.size();

	Eigen::MatrixXd Eigen_B(righe_B,colonne_B);	
	for (int i=0; i<righe_B; ++i) {  
		for (int j=0; j<colonne_B; ++j) {
			Eigen_B(i,j)=B[i][j];
		}
	}

	Eigen::MatrixXd Eigen_R(righe_R,colonne_R);	
	for (int i=0; i<righe_R; ++i) {
		for (int j=0; j<colonne_R; ++j) {
			Eigen_R(i,j)=R[i][j];
		}
	}

	Eigen::VectorXd Eigen_v(dimensione_v);
	for (int i=0; i<dimensione_v; ++i) {
		Eigen_v(i) = v[i];
	}

	Eigen::VectorXd x0 = Eigen::VectorXd::Zero(colonne_B);
	Eigen::MatrixXd A=Eigen_B.transpose()*Eigen_R*Eigen_B; //qua mi dava errore di compilazione perchè avevo scritto Matrix invece di MatrixXd
	Eigen::VectorXd correnti=gradiente_coniugato(A,Eigen_v,x0,tol,it_max,it);

	return correnti;
}


void calcola_tensioni_resistori(const std::vector<std::vector<double>>& B, const std::vector<std::vector<double>>& R, const Eigen::VectorXd& correnti, const undirected_graph<int>& G, const std::map<undirected_edge<int>, ramo>& info_arco) {
	
	Eigen::MatrixXd Eigen_B(B.size(),B[0].size());
	for (int i=0; i<R.size(); i++) {
		for (int j=0; j<B[0].size(); j++) {
			Eigen_B(i,j)=B[i][j];
		}
	}
	
	Eigen::MatrixXd Eigen_R(R.size(), R[0].size());
    for (size_t i = 0; i < R.size(); ++i)
        for (size_t j = 0; j < R[i].size(); ++j)
            Eigen_R(i,j) = R[i][j];
	
	// v_R = R * B * i
	Eigen::VectorXd v_R= Eigen_R * Eigen_B * correnti;
	
	std::vector<ramo> resistori_info;
	for (const auto& arco : G.all_edges()) {
		if (info_arco.count(arco) && info_arco.at(arco).tipologia[0]== 'R') {
			resistori_info.push_back(info_arco.at(arco));
		}
	}
	std::cout << "\n=== TENSIONI E CORRENTI SUI RESISTORI ===\n";
    for (int i = 0; i < (int)R.size(); i++) {
        double tensione = v_R(i);
        double corrente = tensione / resistori_info[i].valore;
        std::cout << resistori_info[i].tipologia
                  << ": V = " << tensione << " volts"
                  << ", I = " << corrente << " amps.\n";
    }
}