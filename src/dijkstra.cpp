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

// Definizione valore infinito per l'inizializzazione del vettore contenente le distanze
const long long INF = numeric_limits<long long>::max();

// Struttura per rappresentare un arco
struct Arco {
    int destinazione;
    int peso;
};

// Struttura per la coda di priorità
struct Nodo {
    long long distanza_corrente;
    int id_nodo;

    // Definzione del metodo su come confrontare due nodi
    bool operator>(const Nodo& other) const {
        return distanza_corrente > other.distanza_corrente;
    }
};

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
