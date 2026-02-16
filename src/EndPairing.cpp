#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

// Struttura per rappresentare un arco con peso
struct Arco {
    int destinazione;
    int peso;
};

// Mappa di adiacenza: adj[u] contiene una lista di {v, peso}
vector<vector<Edge>> adj;
vector<int> circuit;

long long total_cost = 0;

void endPairing(int u) {
    while (!adj[u].empty()) {
        // Prendi l'ultimo arco inserito (LIFO )
        Edge next_edge = adj[u].back();

        // Rimuovi l'arco dal grafo per segnarlo come visitato
        adj[u].pop_back();

        // Ricorsione
        endPairing(next_edge.to);

        //Backtracking
        total_cost += next_edge.weight;
    }

    // Quando il nodo non ha più archi uscenti, lo aggiungiamo al circuito.
    circuit.push_back(u);
}


