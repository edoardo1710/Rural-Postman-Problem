/*
Main file
*/

// Librerie
#include <iostream>
#include <vector>
#include <string>
#include "header.h"

using namespace std;

extern vector<vector<Arco>> adj; 
extern vector<int> circuito;
extern vector<bool> archi_visitati;
extern long long total_cost;

std::string rosso = "\033[31m";
std::string reset = "\033[0m";


// DFS per trovare le componenti connesse
void DFS_FindIslands(int u, vector<bool>& visited, vector<int>& component_nodes, const vector<vector<Arco>>& grafo) {
    visited[u] = true;
    component_nodes.push_back(u);

    for (const auto& arco : grafo[u]) {
        // Navighiamo solo sugli archi necessari per identificare le isole
        if (arco.necessario && !visited[arco.destinazione]) {
            DFS_FindIslands(arco.destinazione, visited, component_nodes, grafo);
        }
    }
}

// Funzione per la pulizia dello schermo
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    // Codice ANSI per pulire lo schermo su Linux/macOS
    std::cout << "\033[2J\033[1;1H";
#endif
}

int main(void) {

    clearScreen();

    int N, M;
    cout << "Inserisci numero di Nodi (N) e Archi (M): ";
    if (!(cin >> N >> M)) return 1;

    // Grafo locale per le manipolazioni iniziali
    vector<vector<Arco>> grafo_locale(N);
    int max_id = 0;

    cout << "Inserisci archi (u v peso 1=Required/0=Optional):" << endl;
    for (int i = 0; i < M; i++) {
        int u, v, w, req;
        cin >> u >> v >> w >> req;
        
        bool necessario = (req == 1);
        
        // Aggiungo arco non orientato (u->v e v->u)
        // Uso 'i' come ID univoco per la coppia di archi
        grafo_locale[u].push_back({v, w, i, necessario});
        grafo_locale[v].push_back({u, w, i, necessario});
        
        max_id = i;
    }

    // FASE 1: Identificazione delle Componenti (Zone Sconnesse)
    std::cout << rosso << "\nFase 1: Identificazione delle Componenti" << reset << std::endl;

    vector<Circuit> isole;
    vector<bool> visited(N, false);
    int island_count = 0;

    // Identifichiamo i nodi che fanno parte di archi necessari
    vector<bool> has_required_edges(N, false);
    for(int i=0; i<N; i++) {
        for(auto& a : grafo_locale[i]) {
            if(a.necessario) has_required_edges[i] = true;
        }
    }

    for (int i = 0; i < N; i++) {
        // Se il nodo ha archi necessari e non è stato visitato, è una nuova isola
        if (has_required_edges[i] && !visited[i]) {
            Circuit nuova_isola;
            nuova_isola.id = island_count++;
            DFS_FindIslands(i, visited, nuova_isola.nodi, grafo_locale);
            isole.push_back(nuova_isola);
        }
    }
    cout << "Trovate " << isole.size() << " isole sconnesse." << endl;

    // FASE 2: Connessione delle Componenti (MST con Z3)
    std::cout << rosso << "\nFase 2: Connessione delle Componenti" << reset << std::endl;

    // Questa funzione aggiungerà ponti required
    grafo_locale = connectComponentsZ3(N, grafo_locale, isole);

    // FASE 3: Matching Ottimo (Parità con Z3)
    std::cout << rosso << "\nFase 3: Matching Ottimo" << reset << std::endl;

    // Questa funzione (dal tuo file MatchingOttimo.cpp) renderà il grafo Euleriano
    grafo_locale = MatchingOttimo(N, grafo_locale);

    // FASE 4: Trova il Circuito Euleriano (Hierholzer)
    std::cout << rosso << "\nFase 4: Individuazione del Circuito Euleriano" << reset << std::endl;

    adj = grafo_locale; 
    circuito.clear();
    total_cost = 0;

    // NUOVA ASSEGNAZIONE ID: Diamo lo stesso ID all'andata e al ritorno dei nuovi ponti
    int current_id = max_id + 1; 
    for(int u = 0; u < N; u++) {
        for(auto& a : adj[u]) {
            if (a.id == -1) {
                a.id = current_id;
                // Cerca il suo gemello per il ritorno e dagli lo stesso ID
                for(auto& gemello : adj[a.destinazione]) {
                    if (gemello.destinazione == u && gemello.id == -1 && gemello.peso == a.peso) {
                        gemello.id = current_id;
                        break;
                    }
                }
                current_id++;
            }
        }
    }
    archi_visitati.assign(current_id, false);

    // Trova nodo di partenza (uno qualsiasi con archi)
    int start_node = 0;
    for(int i=0; i<N; i++) {
        if(!adj[i].empty()) {
            start_node = i;
            break;
        }
    }

    // Lancia l'algoritmo
    endPairingNO(start_node);
    
    // OUTPUT FINALE
    std::cout << rosso << "\n=== RISULTATO ===" << reset << endl;

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

