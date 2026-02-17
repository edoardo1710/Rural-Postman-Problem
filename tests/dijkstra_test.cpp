#include <iostream>
#include <vector>
#include <cassert>
#include <limits>
#include "header.h"

void test_percorso_semplice() {
    std::vector<std::vector<Arco>> grafo(3);
    grafo[0].push_back({1, 50});
    grafo[0].push_back({2, 90});
    grafo[1].push_back({2, 10});

    std::vector<long long> dist = dijkstra(0, grafo);

    // Verifica che il cammino da 0 a 2 abbia distanza 60 e non 90
    assert(dist[2] == 60);
    std::cout << "Test Percorso Semplice Superato." << std::endl;
}

void test_percorso_inesistente() {
    std::vector<std::vector<Arco>> grafo(2);
    std::vector<long long> dist = dijkstra(0, grafo);

    const long long INF = std::numeric_limits<long long>::max();

    // Verifica che il nodo 1 non sia raggiungibile
    assert(dist[1] == INF);
    std::cout << "Test Percorso Inesistente Superato." << std::endl;
}

int main() {
    test_percorso_semplice();
    test_percorso_inesistente();
    std::cout << "Test Superati con Successo." << std::endl;
    return 0;
}