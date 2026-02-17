#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include "header.h"

using namespace std;


// Mappa di adiacenza: adj[u] contiene una lista di {v, peso}
vector<vector<Arco>> adj;
vector<int> circuito;
vector<bool> archi_visitati;

long long total_cost = 0;

void endPairingNO(int u) {
    while (!adj[u].empty()) {
        // Prendi l'ultimo arco inserito (LIFO )
        Arco next = adj[u].back();

        // Rimuovi l'arco dal grafo per segnarlo come visitato
        adj[u].pop_back();

        // Se questo arco (identificato dal suo ID) � gi� stato percorso
        // (magari venendo dall'altra direzione), lo saltiamo.
        if (archi_visitati[next.id]) {
            continue;
        }

        // Segna l'arco come visitato globalmente
        archi_visitati[next.id] = true;

        // Ricorsione
        endPairingNO(next.destinazione);

        //Backtracking
        total_cost += next.peso;
    }

    // Quando il nodo non ha pi� archi uscenti, lo aggiungiamo al circuito.
    circuito.push_back(u);
}

