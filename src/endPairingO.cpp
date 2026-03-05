#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include "header.h"

using namespace std;

vector<vector<Arco>> adj;
vector<int> circuito;

long long total_cost = 0;

void endPairingO(int u) {
    while (!adj[u].empty()) {
        // Prendi l'ultimo arco inserito (LIFO)
        Arco next= adj[u].back();

        // Rimuovi l'arco dal grafo per segnarlo come visitato
        adj[u].pop_back();

        // Ricorsione
        endPairingO(next.destinazione);

        //Backtracking
        total_cost += next.peso;
    }

    // Quando il nodo non ha più archi uscenti, lo aggiungiamo al circuito.
    circuito.push_back(u);
}




