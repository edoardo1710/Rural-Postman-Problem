#include "EndPairing.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

int main() {
    int N, A;

    cout << "Inserisci numero Nodi e Archi: ";
    if (!(cin >> N >> A)||!((N!=0)||(A!=0))) return 0;

    adj.resize(N + 1);//Contatore da 1 a N

    vector<int> in_degree(N + 1, 0);
    vector<int> out_degree(N + 1, 0);

    cout << "Inserisci archi (nodo di partenza, nodo di arrivo, costo):" << endl;
    for (int i = 0; i < A; ++i) {
        int x, y, c;
        cin >> x >> y >> c;

        adj[x].push_back({y, c});

        out_degree[x]++;
        in_degree[y]++;
    }
    // Verifica che il grafo sia Euleriano
    int start_node = 0;
    for (int i = 1; i <= N; ++i) {
        if ((out_degree[i]+in_degree[i])%2!=0) {
            cout << "ERRORE: Il grafo NON è Euleriano (Nodo " << i << " dispari)." << endl;
            return 1;
            // implementare matching bipartito
        }
        if (out_degree[i] > 0 && start_node == 0) {
            start_node = i;
        }
    }

    if (start_node == 0) {
        cout << "Il grafo non ha archi." << endl;
        return 0;
    }

    findEulerianCircuit(start_node);

    reverse(circuit.begin(), circuit.end());//Perchè eseguiamo un backtracking

    cout << "\n--- Risultato ---" << endl;
    cout << "Costo Totale del Circuito: " << total_cost << endl;

    cout << "Percorso: ";
    for (size_t i = 0; i < circuit.size(); ++i) {
        cout << circuit[i];
        if (i < circuit.size() - 1) cout << " -> ";
    }
    cout << endl;

    if (circuit.size() != M + 1) {
        cout << "ATTENZIONE: Il grafo ha più componenti connesse. Il circuito non copre tutti gli archi." << endl;
    }

    return 0;
}
