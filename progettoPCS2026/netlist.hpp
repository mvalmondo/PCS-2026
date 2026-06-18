#pragma once
#include <string>
#include <vector>
#include <map>
#include <set>
#include <iostream>
#include <fstream>
#include <sstream>


struct ramo { //preso da file es5
	std::string tipologia;
	double valore; 
	int nodo_partenza;
	int nodo_arrivo;
};

std::vector<ramo> leggi_netlist(const std::string& nome_file) {
	std::vector<ramo> netlist;
	std::ifstream file(nome_file);
	
	if (!file.is_open()) {
		std::cerr << "Il file non è stato aperto." << std::endl;
		std::exit(EXIT_FAILURE);
	}
	
	// se il file si apre riempiamo lo struct
	std::string riga;
	
	while (std::getline(file,riga)) {
		if (riga.empty()) {
			continue;
		}
		
		std::stringstream ss(riga);
		ramo componente;
		if (ss >> componente.tipologia >> componente.valore >> componente.nodo_partenza >> componente.nodo_arrivo) {
			netlist.push_back(componente);
		} else {
			std::cerr << "Errore indentazione riga -> " << riga << std::endl;
		}
	}
	
	file.close();
	return netlist;
};



