/*
Algoritmo per la connessione dei circuiti
*/

// Librerie
#include "header.h"
#include "z3++.h" 
#include <vector>
#include <limits>
#include <iostream>

using namespace std;
using namespace z3;

vector<vector<Arco>> connectComponentsZ3(int N, vector<vector<Arco>> adj, const vector<Circuit>& isole) {

    // Numero di circuiti (isole)
    int K = isole.size();
    if (K <= 1) return adj;

    cout << "Calcolo distanze tra le " << K << " isole..." << endl;

    // Salviamo il costo e il "ponte" (l'arco)
    vector<vector<long long>> macro_grafo(K, vector<long long>(K, INF));
    vector<vector<pair<int, int>>> best_bridge(K, vector<pair<int, int>>(K, {-1, -1}));

    // Ciclo sulle isole
    for(int i = 0; i < K; i++) {
        
        for(int nodo_partenza : isole[i].nodi) {
            
            // Lancio Dijkstra
            auto result = dijkstra(nodo_partenza, adj);
            vector<long long>& distanze = result.first;

            // Cerco la distanza minima verso le altre isole
            for(int j = 0; j < K; j++) {
                if (i == j) continue;
                
                for(int nodo_arrivo : isole[j].nodi) {

                    if (distanze[nodo_arrivo] < macro_grafo[i][j]) {
                        
                        // Aggiorno costo
                        macro_grafo[i][j] = distanze[nodo_arrivo];
                        
                        // Aggiorno l'arco
                        best_bridge[i][j] = {nodo_partenza, nodo_arrivo};
                    }
                }
            }
        }
    }

    // Parte Z3 per individuare i collegamenti milgiori
    cout << "Avvio Z3 per scegliere i collegamenti ottimi..." << endl;
    
    context c;
    optimize opt(c);

    // Variabili: x[i][j] (binaria), f[i][j] (flusso)
    vector<vector<expr>> x, f;

    // Ciclo nuovamente sulle isole
    for (int i = 0; i < K; ++i) {

        vector<expr> x_row, f_row;

        for (int j = 0; j < K; ++j) {

            x_row.push_back(c.int_const(("x_" + to_string(i) + "_" + to_string(j)).c_str()));
            f_row.push_back(c.int_const(("f_" + to_string(i) + "_" + to_string(j)).c_str()));

            // Vincoli base
            opt.add(x_row.back() >= 0 && x_row.back() <= 1);
            opt.add(f_row.back() >= 0 && f_row.back() <= K);
            
            // Se non c'è percorso possibile, forza a 0
            if (macro_grafo[i][j] == INF) {
                opt.add(x_row.back() == 0);
            }
            // Se c'è flusso, deve esserci l'arco attivo
            opt.add(f_row.back() <= K * x_row.back());
        }
        
        x.push_back(x_row);
        f.push_back(f_row);
    }

    // Isola 0: OUT - IN = K - 1
    {
        expr flow_out = c.int_val(0), flow_in = c.int_val(0);
        for(int j=1; j<K; ++j) { 
            
            flow_out = flow_out + f[0][j]; flow_in = flow_in + f[j][0]; 
        }

        opt.add(flow_out - flow_in == (K - 1));
    }

    // Altre Isole: IN - OUT = 1
    for(int i=1; i<K; ++i) {

        expr flow_out = c.int_val(0), flow_in = c.int_val(0);
        for(int j=0; j<K; ++j) { 
            
            if(i!=j) { 
                
                flow_out = flow_out + f[i][j]; flow_in = flow_in + f[j][i]; 
            } 
        }

        opt.add(flow_in - flow_out == 1);
    }
    
    // Simmetria (Grafo non orientato)
    for(int i=0; i<K; ++i)
        for(int j=i+1; j<K; ++j)
            opt.add(x[i][j] == x[j][i]);

    // Minimizzazione costi
    expr total_cost = c.int_val(0);

    for(int i=0; i<K; ++i)
        for(int j=i+1; j<K; ++j)
            if(macro_grafo[i][j] != INF)
                total_cost = total_cost + (x[i][j] * (int)macro_grafo[i][j]);
    
    opt.minimize(total_cost);

    // Ricostruzione grafi
    if (opt.check() == sat) {

        model m = opt.get_model();

        for (int i = 0; i < K; ++i) {

            for (int j = i + 1; j < K; ++j) {

                if (m.eval(x[i][j]).get_numeral_int() == 1) {
                    
                    // Recuperiamo i nodi reali che avevamo salvato
                    int u = best_bridge[i][j].first;
                    int v = best_bridge[i][j].second;

                    cout << "Collego Isola " << i << " e " << j << " (Nodi " << u << " -> " << v << ")" << endl;

                    // Aggiunta del percorso fisico al grafo
                    auto res = dijkstra(u, adj);
                    vector<int> parents = res.second;

                    int curr = v;

                    while (curr != u) {

                        int prev = parents[curr];
                        int peso = 0;
                        // Trova peso originale
                        for(auto& a : adj[prev]) if(a.destinazione == curr) peso = a.peso;
                        
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
     // Ritorno il grafo
    return adj;
}