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
    int N, M;
    cout << "Inserisci numero di Nodi (N) e Archi (M): ";
    if (!(cin >> N >> M)) return 0;

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

    // -----------------------------------------------------------
    // FASE 1: Identificazione delle Componenti (Zone Sconnesse)
    // -----------------------------------------------------------
    cout << "\n--- FASE 1: Identificazione Isole (Zone) ---" << endl;
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

    // -----------------------------------------------------------
    // FASE 2: Connessione delle Componenti (MST con Z3)
    // -----------------------------------------------------------
    cout << "\n--- FASE 2: Connessione Isole (Z3) ---" << endl;
    // Questa funzione (dal tuo file connessione.cpp) aggiungerà ponti required
    grafo_locale = connectComponentsZ3(N, grafo_locale, isole);

    // -----------------------------------------------------------
    // FASE 3: Matching Ottimo (Parità con Z3)
    // -----------------------------------------------------------
    cout << "\n--- FASE 3: Matching Ottimo (Z3) ---" << endl;
    // Questa funzione (dal tuo file MatchingOttimo.cpp) renderà il grafo Euleriano
    grafo_locale = MatchingOttimo(N, grafo_locale);

    // -----------------------------------------------------------
    // FASE 4: Trova il Circuito Euleriano (Hierholzer)
    // -----------------------------------------------------------
    cout << "\n--- FASE 4: Calcolo Circuito Finale ---" << endl;

    // A. Preparazione variabili globali per EndPairingNO
    adj = grafo_locale; // Copiamo il grafo lavorato nella variabile globale
    circuito.clear();
    // Ridimensioniamo il vettore visitati. 
    // Nota: MatchingOttimo potrebbe aver aggiunto archi con ID -1 o duplicati.
    // Per sicurezza, resettiamo la logica degli ID o usiamo un approccio sicuro.
    // Poiché EndPairingNO usa archi_visitati[id], dobbiamo assicurarci che gli ID siano validi.
    // Riassegniamo gli ID univoci a tutti gli archi attuali per evitare crash.
    
    int current_id = 0;
    for(int u=0; u<N; u++) {
        for(auto& a : adj[u]) {
            a.id = current_id++; // ID univoco temporaneo per la visita
        }
    }
    archi_visitati.assign(current_id + 1, false);

    // B. Trova nodo di partenza (uno qualsiasi con archi)
    int start_node = 0;
    for(int i=0; i<N; i++) {
        if(!adj[i].empty()) {
            start_node = i;
            break;
        }
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

