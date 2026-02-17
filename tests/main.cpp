#include <iostream>
#include <vector>
#include "header.h"

using namespace std;

// --- VARIABILI GLOBALI (Necessarie per EndPairingNO.cpp) ---
// Queste variabili sono definite negli altri file .cpp, qui le colleghiamo
extern vector<vector<Arco>> adj; 
extern vector<int> circuito;
extern vector<bool> archi_visitati;
extern long long total_cost; // Se presente in EndPairingNO

// --- FUNZIONI AUSILIARIE PER IL MAIN ---

// DFS per trovare le componenti connesse (le "zone" o "isole")
void DFS_FindIslands(int u, vector<bool>& visited, vector<int>& component_nodes, const vector<vector<Arco>>& grafo) {
    visited[u] = true;
    component_nodes.push_back(u);

    for (const auto& arco : grafo[u]) {
        // Navighiamo solo sugli archi NECESSARI per identificare le isole
        if (arco.necessario && !visited[arco.destinazione]) {
            DFS_FindIslands(arco.destinazione, visited, component_nodes, grafo);
        }
    }
}

int main() {
    // 1. Inizializzazione del Grafo
    // Supponiamo di caricare i dati da un file o input manuale
    vector<Nodo> nodi;
    vector<Arco> archi_obbligatori; // Archi che DEVONO essere percorsi (RPP)
    vector<Arco> archi_opzionali;   // Archi percorribili per collegamento
    
    cout << "--- Risolutore Problema del Postino Rurale ---" << endl;

    // 2. Controllo Connessione (connessione.cpp)
    // Verifica se il grafo formato dagli archi obbligatori è connesso
    if (!controllaConnessione(archi_obbligatori)) {
        cout << "Il grafo degli archi obbligatori non è connesso. "
             << "Utilizzo Dijkstra per trovare i collegamenti minimi..." << endl;
        // Chiamate a funzioni in connessione.cpp per aggiungere archi di collegamento
    }

    // 3. Calcolo Gradi e Identificazione Nodi Dispari
    // Per il postino rurale, dobbiamo rendere il grafo Euleriano
    vector<int> nodi_dispari = trovaNodiDispari(archi_obbligatori);

    // 4. Calcolo Distanze tra Nodi Dispari (dijkstra.cpp)
    // Se ci sono nodi con grado dispari, serve un matching
    if (!nodi_dispari.empty()) {
        vector<vector<double>> matrice_distanze;
        for (int u : nodi_dispari) {
            // Esegue Dijkstra per ogni nodo dispari per trovare il cammino minimo verso gli altri
            matrice_distanze.push_back(eseguiDijkstra(u, archi_opzionali));
        }

        // 5. Matching Ottimo (MatchingOttimo.cpp)
        // Trova l'accoppiamento di costo minimo tra i nodi dispari
        vector<Arco> archi_matching = calcolaMatchingOttimo(nodi_dispari, matrice_distanze);
        
        // Aggiunge gli archi del matching per rendere tutti i gradi pari
        archi_obbligatori.insert(archi_obbligatori.end(), archi_matching.begin(), archi_matching.end());
    }

    // C. Lancia l'algoritmo
    // Assumiamo grafo Non Orientato (NO) come da contesto RPP standard
    endPairingNO(start_node);

    // -----------------------------------------------------------
    // OUTPUT FINALE
    // -----------------------------------------------------------
    cout << "\n=== RISULTATO ===" << endl;
    cout << "Percorso: ";
    // Il circuito è in ordine inverso (o corretto a seconda dell'implementazione), lo stampiamo
    for (size_t i = 0; i < circuito.size(); ++i) {
        cout << circuito[i];
        if (i != circuito.size() - 1) cout << " -> ";
    }
    cout << endl;
    cout << "Costo Totale Calcolato: " << total_cost << endl;

    return 0;
}

