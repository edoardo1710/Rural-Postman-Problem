/*
Codice algoritmo di Djikstra
*/

// Librerie
#include "header.h"
#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <utility>

using namespace std;


pair<vector<long long>, vector<int>> dijkstra(int sorgente, const vector<vector<Arco>>& adj) {

    // n = |V|, numero di nodi nel grafo
    int n = adj.size();

    // Inizializzazione delle distanze e dei predecessori
    vector<long long> dist(n, INF);
    vector<int> parent(n, -1);

    // Costruzione Min-Heap
    priority_queue<Nodo, vector<Nodo>, greater<Nodo>> pq;

    // Inizializziamo la sorgente
    dist[sorgente] = 0;
    pq.push({0, sorgente});

    // Ciclo sulla coda
    while(!pq.empty()) {

        // Estraggo l'elemento più piccolo
        long long d = pq.top().distanza_corrente;
        int u = pq.top().id_nodo;
        pq.pop();

        // Primo caso: se la distanza fra la sorgente e il nodo corrente è già quella minima ignoriamo e saltiamo alla prossima iterazione
        if(d > dist[u]) continue;

        // Secondo caso: esploriamo i nodi vicini ed effettuiamo l'operazione di rilassamento dove necessario
        for(const auto& arco : adj[u]) {

            // Rilassamento
            if(dist[u] + arco.peso < dist[arco.destinazione]) {
                dist[arco.destinazione] = dist[u] + arco.peso;
                parent[arco.destinazione] = u;
                pq.push({dist[arco.destinazione], arco.destinazione});
            }
        }

    }

    // Ritorno il vettore delle distanze
    return {dist, parent};
}
