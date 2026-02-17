/*
Codice connessione circuiti euleriani
*/

#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include "header.h"

using namespace std;

// Funzione per il calcolo delle distanze fra i circuiti
vector<vector<long long>> connection(const vector<Circuit>& isole, const vector<vector<Arco>>& adj) {

    int K = isole.size();
    vector<vector<long long>> macro_grafo(K, vector<long long>(K, INF));

    for(int i = 0; i < K; i++) {
        // PER OGNI NODO DELL'ISOLA I...
        for(int nodo_partenza : isole[i].nodi) {
            
            // ...LANCIO IL DIJKSTRA NORMALE!
            auto [distanze, parent] = dijkstra(nodo_partenza, adj);

            // Cerco la distanza minima verso le altre isole
            for(int j = 0; j < K; j++) {
                if (i == j) {
                    macro_grafo[i][j] = 0;
                    continue;
                }
                
                for(int nodo_arrivo : isole[j].nodi) {
                    if (distanze[nodo_arrivo] < macro_grafo[i][j]) {
                        macro_grafo[i][j] = distanze[nodo_arrivo];
                    }
                }
            }
        }
    }
    return macro_grafo;
}

