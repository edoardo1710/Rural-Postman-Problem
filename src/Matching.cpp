#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <limits>
#include "header.h"

using namespace std;

vector<vector<Arco>> matching(int N, vector<vector<Arco>> adj) {

    // A. Identifica i nodi con grado dispari
    vector<int> odds;
    for (int i = 0; i < N; i++) {
        if ((out_degree[i]+in_degree[i])%2!=0) {
            odds.push_back(i);
        }
    }

    if (odds.empty()) {
        cout << "Il grafo e' gia' Euleriano." << endl;
        return adj;
    }

    int K = odds.size();
    cout << "Trovati " << K << " nodi dispari. Inizio il matching ottimo..." << endl;

    vector<vector<long long>> dist_matrix(K, vector<long long>(K));
    vector<vector<int>> parent_matrix(K);

    for (int i = 0; i < K; i++) {

        pair<vector<long long>, vector<int>> result = dijkstra(odds[i], adj);

        parent_matrix[i] = result.second;

        for (int j = 0; j < K; j++) {
            dist_matrix[i][j] = result.first[odds[j]];
        }
    }

    int limit = (1 << K);
    vector<long long> dp(limit, INF);
    vector<int> parent_mask(limit, -1);
    dp[0] = 0;

    for (int mask = 0; mask < limit; mask++) {
        if (dp[mask] == INF) continue;

        // Trova il primo nodo dispari libero (bit a 0)
        int i = -1;
        for (int n = 0; n < K; n++) {
            if (!((mask >> n) & 1)) {
                i = n;
                break;
            }
        }
        if (i == -1) continue; // Tutti accoppiati

        // Prova ad accoppiare odds[i] con odds[j]
        for (int j = i + 1; j < K; j++) {
            if (!((mask >> j) & 1)) {
                int next_mask = mask | (1 << i) | (1 << j);
                long long cost = dist_matrix[i][j];

                if (dp[mask] + cost < dp[next_mask]) {
                    dp[next_mask] = dp[mask] + cost;
                    parent_mask[next_mask] = mask; // Mi ricordo lo stato precedente
                }
            }
        }
    }

    // D. Ricostruzione e Modifica del Grafo
    int curr_mask = limit - 1;

    // Se non siamo riusciti a chiudere il matching (grafo sconnesso)
    if (dp[curr_mask] == INF) {
        cout << "Impossibile rendere il grafo Euleriano (grafo sconnesso)." << endl;
        return adj;
    }

    while (curr_mask > 0) {
        int prev = parent_mask[curr_mask];
        int diff = curr_mask ^ prev; // I bit cambiati indicano la coppia scelta

        int idx1 = -1, idx2 = -1;
        for (int k = 0; k < K; k++) {
            if ((diff >> k) & 1) {
                if (idx1 == -1) idx1 = k;
                else idx2 = k;
            }
        }

        // Abbiamo deciso di accoppiare odds[idx1] e odds[idx2]
        int start_node = odds[idx1];
        int end_node = odds[idx2];

        // Recuperiamo il percorso fisico usando il vettore parent salvato prima
        // Usiamo i padri calcolati da Dijkstra partendo da start_node
        const vector<int>& parents = parent_matrix[idx1];

        int curr = end_node;
        while (curr != start_node) {
            int prev_node = parents[curr];

            // Trovo il peso dell'arco originale tra prev_node e curr
            int weight = 0;
            for(const auto& a : adj[prev_node]) {
                if(a.destinazione == curr) {
                    weight = a.peso;
                    break;
                }
            }

            // AGGIUNTA ARCO (Duplicazione)
            // Aggiungiamo l'arco in entrambe le direzioni
            adj[prev_node].push_back({curr, weight});
            adj[curr].push_back({prev_node, weight});

            curr = prev_node; // Torno indietro
        }

        curr_mask = prev;
    }

    return adj;
}
