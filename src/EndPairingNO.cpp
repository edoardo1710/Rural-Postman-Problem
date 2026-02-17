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
    while (!adj[u].empty()) {
        Arco next = adj[u].back();
        adj[u].pop_back();

        // 1. FONDAMENTALE: Se la strada non è necessaria (opzionale scartata), ignorala!
        if (!next.necessario) {
            continue;
        }

        // 2. Se l'abbiamo già percorsa (in questa o nell'altra direzione), ignorala!
        if (archi_visitati[next.id]) {
            continue;
        }

        archi_visitati[next.id] = true;
        endPairingNO(next.destinazione);
        total_cost += next.peso;
    }

    circuito.push_back(u);
}