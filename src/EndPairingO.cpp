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
vector<vector<Arco>> adj;
vector<int> circuito;

long long total_cost = 0;

void endPairingO(int u) {
    while (!adj[u].empty()) {
        // Prendi l'ultimo arco inserito (LIFO )
        Arco next= adj[u].back();

        // Rimuovi l'arco dal grafo per segnarlo come visitato
        adj[u].pop_back();

        // Ricorsione
        endPairing(next.destinazione);

        //Backtracking
        total_cost += next.peso;
    }

    // Quando il nodo non ha più archi uscenti, lo aggiungiamo al circuito.
    circuito.push_back(u);
}


