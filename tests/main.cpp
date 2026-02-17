#include <iostream>
#include <vector>
#include <string>
#include "header.h" // Include le definizioni di Arco, Nodo e i prototipi

using namespace std;

int main() {
    // 1. Inizializzazione del Grafo
    // Supponiamo di caricare i dati da un file o input manuale
    vector<Nodo> nodi;
    vector<Arco> archi_obbligatori; // Archi che DEVONO essere percorsi (RPP)
    vector<Arco> archi_opzionali;   // Archi percorribili per collegamento
    
    cout << "--- Risolutore Problema del Postino Rurale ---" << endl;

    // 2. Controllo Connessione (connessione.cpp)
    // Verifica se il grafo formato dagli archi obbligatori è connesso
    if (!connectComponentsZ3(5, [])) {
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

    // 6. Generazione del Ciclo Finale (EndPairing.cpp)
    // Una volta che il grafo è connesso e tutti i nodi hanno grado pari
    cout << "Generazione del percorso ottimale..." << endl;
    vector<int> percorso_finale = generaCicloEuleriano(archi_obbligatori);

    // 7. Output del risultato
    cout << "Percorso calcolato con successo!" << endl;
    for (int nodo_id : percorso_finale) {
        cout << nodo_id << " -> ";
    }
    cout << "FINE" << endl;

    return 0;
}
