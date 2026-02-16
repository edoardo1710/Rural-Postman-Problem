#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <vector>

struct Arco {
    int destinazione;
    int peso;
};

std::vector<long long> dijkstra(int sorgente, const std::vector<std::vector<Arco>>& adj);

#endif