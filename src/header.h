/*
File Header
*/

#pragma once
#include <iostream>
#include <limits>
#include <vector>
#include <algorithm>
#include <utility>
#include <map>

// Definizione valore infinito
const long long INF = std::numeric_limits<long long>::max();

// Definizione della struttura "Arco"
struct Arco {
    int destinazione;
    int peso;
    int id;
    bool necessario;
};

// Definizione della struttura "Nodo"
struct Nodo {
    long long distanza_corrente;
    int id_nodo;

    // Definzione del metodo su come confrontare due nodi
    bool operator>(const Nodo& other) const {
        return distanza_corrente > other.distanza_corrente;
    }
};

// // Definizione della struttura "Circuit"
struct Circuit {
    int id;
    std::vector<int> nodi;
};

// Algoritmo di Dijkstra
std::pair<std::vector<long long>, std::vector<int>> dijkstra(int sorgente, const std::vector<std::vector<Arco>>& adj);

// Algoritmo per collegamento circuiti
std::vector<std::vector<Arco>> connectComponentsZ3(int N, std::vector<std::vector<Arco>> adj, const std::vector<Circuit>& isole);

// Algoritmo per il pairing per grafi orientati
void endPairingO(int u);

// Algoritmo per il pairing per grafi non orientati
void endPairingNO(int u);