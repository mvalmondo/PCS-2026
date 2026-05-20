#include "header_es9.hpp"
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


	
	// Visita BFS
	fifo<int> q;
	auto bfsG = graph_visit(G, 1, q);
	
	// Visita DFS iterativa
	lifo<int> s;
	auto dfsG = graph_visit(G, 1, s);
	
	// DFS ricorsiva
	auto rdfsG = recursive_dfs(G, 1);
	
	// Dijkstra
	auto dijkG = dijkstra(G, 1);
	
	// Stampa tutti i grafi in formato DOT
	print_dot(G,     "grafo.dot",    "Originale");
	print_dot(bfsG,  "bfs.dot",      "BFS");
	print_dot(dfsG,  "dfs.dot",      "DFS");
	print_dot(rdfsG, "rdfs.dot",     "DFS_ricorsiva");
	print_dot(dijkG, "dijkstra.dot", "Dijkstra");
	return 0;
}

