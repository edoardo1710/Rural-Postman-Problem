/*
Codice Algoritmo Pairing per grafi non orientati
*/

// Librerie
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include "header.h"

using namespace std;

vector<vector<Arco>> adj;
vector<int> circuito;
vector<bool> archi_visitati;

long long total_cost = 0;

void endPairingNO(int u) {

    // Ciclo sul grafo
    while (!adj[u].empty()) {

        Arco next = adj[u].back();
        adj[u].pop_back();

        // Primo caso: se la strada non è necessaria viene ignorata
        if (!next.necessario) {
            continue;
        }

        // Secondo caso: se la strada l'abbiamo già percorsa viene ignorata
        if (archi_visitati[next.id]) {
            continue;
        }

        // Pairing
        archi_visitati[next.id] = true;
        endPairingNO(next.destinazione);
        total_cost += next.peso;
    }

    // Ritorno il circuito
    circuito.push_back(u);
}