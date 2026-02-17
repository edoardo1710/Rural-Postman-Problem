#include "header.h"
#include <z3++.h> 
#include <vector>
#include <limits>
#include <iostream>

using namespace std;
using namespace z3;

// Funzione modificata: Restituisce il GRAFO AGGIORNATO con i ponti aggiunti
vector<vector<Arco>> connectComponentsZ3(int N, vector<vector<Arco>> adj, const vector<Circuit>& isole) {

    int K = isole.size();
    if (K <= 1) return adj; // Se c'è una sola isola, siamo a posto.

    cout << "Calcolo distanze tra le " << K << " isole..." << endl;

    // --- PARTE 1: IL TUO CODICE (CON UNA MODIFICA) ---
    // Oltre al costo, salviamo COSA collega i e j: best_bridge[i][j] = {nodo_u, nodo_v}
    
    vector<vector<long long>> macro_grafo(K, vector<long long>(K, INF));
    vector<vector<pair<int, int>>> best_bridge(K, vector<pair<int, int>>(K, {-1, -1}));

    for(int i = 0; i < K; i++) {
        // Ottimizzazione: Non serve lanciare Dijkstra da TUTTI i nodi.
        // Basta lanciarlo dai nodi di 'i', ma possiamo ottimizzare se le isole sono grandi.
        // Per ora teniamo il tuo approccio (sicuro ma lento se i nodi sono tanti).
        
        for(int nodo_partenza : isole[i].nodi) {
            
            // Lancio Dijkstra (assumo che dijkstra sia definita in eulero.h o inclusa)
            auto result = dijkstra(nodo_partenza, adj);
            vector<long long>& distanze = result.first;

            // Cerco la distanza minima verso le altre isole
            for(int j = 0; j < K; j++) {
                if (i == j) continue; // Distanza 0 verso se stessi
                
                for(int nodo_arrivo : isole[j].nodi) {
                    if (distanze[nodo_arrivo] < macro_grafo[i][j]) {
                        
                        // AGGIORNO COSTO (Come facevi tu)
                        macro_grafo[i][j] = distanze[nodo_arrivo];
                        
                        // AGGIORNO MEMORIA (Nuovo: mi segno i nodi del ponte)
                        best_bridge[i][j] = {nodo_partenza, nodo_arrivo};
                    }
                }
            }
        }
    }

    // --- PARTE 2: Z3 SOLVER (SCEGLIE I PONTI MIGLIORI) ---
    cout << "Avvio Z3 per scegliere i collegamenti ottimi..." << endl;
    
    context c;
    optimize opt(c);

    // Variabili: x[i][j] (binaria: collego i e j?), f[i][j] (flusso)
    vector<vector<expr>> x, f;

    for (int i = 0; i < K; ++i) {
        vector<expr> x_row, f_row;
        for (int j = 0; j < K; ++j) {
            x_row.push_back(c.int_const(("x_" + to_string(i) + "_" + to_string(j)).c_str()));
            f_row.push_back(c.int_const(("f_" + to_string(i) + "_" + to_string(j)).c_str()));

            // Vincoli base
            opt.add(x_row.back() >= 0 && x_row.back() <= 1);
            opt.add(f_row.back() >= 0 && f_row.back() <= K);
            
            // Se non c'è percorso possibile (INF), forza a 0
            if (macro_grafo[i][j] == INF) {
                opt.add(x_row.back() == 0);
            }
            // Se c'è flusso, deve esserci l'arco attivo
            opt.add(f_row.back() <= K * x_row.back());
        }
        x.push_back(x_row);
        f.push_back(f_row);
    }

    // Vincolo Flusso (Sorgente 0 manda a tutti gli altri)
    // Isola 0: OUT - IN = K - 1
    {
        expr flow_out = c.int_val(0), flow_in = c.int_val(0);
        for(int j=1; j<K; ++j) { flow_out = flow_out + f[0][j]; flow_in = flow_in + f[j][0]; }
        opt.add(flow_out - flow_in == (K - 1));
    }
    // Altre Isole: IN - OUT = 1
    for(int i=1; i<K; ++i) {
        expr flow_out = c.int_val(0), flow_in = c.int_val(0);
        for(int j=0; j<K; ++j) { if(i!=j) { flow_out = flow_out + f[i][j]; flow_in = flow_in + f[j][i]; } }
        opt.add(flow_in - flow_out == 1);
    }
    
    // Simmetria (Grafo non orientato)
    for(int i=0; i<K; ++i)
        for(int j=i+1; j<K; ++j)
            opt.add(x[i][j] == x[j][i]);

    // OBIETTIVO: Minimizzare costi
    expr total_cost = c.int_val(0);
    for(int i=0; i<K; ++i)
        for(int j=i+1; j<K; ++j)
            if(macro_grafo[i][j] != INF)
                total_cost = total_cost + (x[i][j] * (int)macro_grafo[i][j]);
    
    opt.minimize(total_cost);

    // --- PARTE 3: RICOSTRUZIONE ---
    if (opt.check() == sat) {
        model m = opt.get_model();
        for (int i = 0; i < K; ++i) {
            for (int j = i + 1; j < K; ++j) {
                if (m.eval(x[i][j]).get_numeral_int() == 1) {
                    
                    // Recuperiamo i nodi reali che avevamo salvato
                    int u = best_bridge[i][j].first;
                    int v = best_bridge[i][j].second;

                    cout << "Collego Isola " << i << " e " << j << " (Nodi " << u << " -> " << v << ")" << endl;

                    // Aggiungiamo il percorso fisico al grafo
                    // Dobbiamo rifare Dijkstra O recuperare i padri se li avessimo salvati (ma occupa troppa RAM salvarli tutti)
                    // Rifare Dijkstra una tantum qui è veloce.
                    auto res = dijkstra(u, adj);
                    vector<int> parents = res.second;

                    int curr = v;
                    while (curr != u) {
                        int prev = parents[curr];
                        int peso = 0;
                        // Trova peso originale
                        for(auto& a : adj[prev]) if(a.destinazione == curr) peso = a.peso;
                        
                        // AGGIUNGO ARCO REQUIRED
                        adj[prev].push_back({curr, peso, -1, true});
                        adj[curr].push_back({prev, peso, -1, true});
                        
                        curr = prev;
                    }
                }
            }
        }
    } else {
        cout << "Errore: Impossibile connettere le isole." << endl;
    }

    return adj;
}