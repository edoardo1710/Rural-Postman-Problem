/*
Algoritmo per Matching ottimo
*/

// Librerie
#include "header.h"
#include "z3++.h"
#include <string>

using namespace std;
using namespace z3;

vector<vector<Arco>> MatchingOttimo(int N, vector<vector<Arco>> adj) {

    vector<int> nodi_dispari;

    for (int i = 0; i < N; i++) {

        int grado_necessario = 0;
        for(const auto& a : adj[i]) {
            if(a.necessario) grado_necessario++;
        }
        if (grado_necessario % 2 != 0) {
            nodi_dispari.push_back(i);
        }
    }

    if (nodi_dispari.empty()) {
        cout << "Il grafo e' gia' Euleriano (tutti i nodi hanno grado pari)." << endl;
        return adj;
    }

    int D = nodi_dispari.size();
    cout << "Trovati " << D << " nodi dispari. Calcolo percorsi minimi e avvio Z3..." << endl;

    vector<vector<long long>> distanze_matrice(D, vector<long long>(D));
    vector<vector<int>> padri_matrice(D);

    for (int i = 0; i < D; i++) {

        // Lanciamo Dijkstra dal nodo dispari nodidispari[i]
        pair<vector<long long>, vector<int>> result = dijkstra(nodi_dispari[i], adj);

        padri_matrice[i] = result.second; // Salviamo i padri

        for (int j = 0; j < D; j++) {
            distanze_matrice[i][j] = result.first[nodi_dispari[j]];
        }
    }

    // Implementazione Z3 (Constraint Programming)
    context c;
    optimize opt(c);

    // Matrice di variabili decisionali Z3: x[i][j]
    // x[i][j] = 1 se accoppiamo il nodo dispari i con j, 0 altrimenti
    vector<vector<expr>> x;

    // Creazione variabili e vincoli base
    for (int i = 0; i < D; ++i) {

        vector<expr> riga;

        for (int j = 0; j < D; ++j) {

            // Variabili che z3 dovrà "indovinare".
            string name = "x_" + to_string(i) + "_" + to_string(j);
            riga.push_back(c.int_const(name.c_str()));

            // Vincolo: Binario (0 o 1)
            opt.add(riga.back() >= 0 && riga.back() <= 1);

            // Vincolo: Se la distanza è INF (irraggiungibile), non accoppiare
            if (i == j || distanze_matrice[i][j] == INF) {
                opt.add(riga.back() == 0);
            }
        }
        x.push_back(riga);
    }

    // Vincolo: Simmetria (x_ij == x_ji)
    for (int i = 0; i < D; ++i) {
        for (int j = i + 1; j < D; ++j) {
            opt.add(x[i][j] == x[j][i]);
        }
    }

    // Ogni nodo i deve essere collegato a esattamente un altro nodo j
    for (int i = 0; i < D; ++i) {

        expr somma_riga = c.int_val(0);

        for (int j = 0; j < D; ++j) {
            if (i != j) {
                somma_riga = somma_riga + x[i][j];
            }
        }
        opt.add(somma_riga == 1);
    }

    // Minimizzazione del costo totale
    expr costo_totale = c.int_val(0);

    for (int i = 0; i < D; ++i) {

        for (int j = i + 1; j < D; ++j) {
            if (distanze_matrice[i][j] < INF) {
                // Aggiungiamo al costo totale: variabile * distanza
                costo_totale = costo_totale + (x[i][j] * (int)distanze_matrice[i][j]);
            }
        }
    }

   // Z3 minimizza costo_totale
    opt.minimize(costo_totale);

    if (opt.check() == sat) {

        cout << "Matching ottimo trovato." << endl;
        model m = opt.get_model();

        // Iteriamo solo sulla metà superiore della matrice per non processare le coppie due volte
        for (int i = 0; i < D; ++i) {
            for (int j = i + 1; j < D; ++j) {

                // Se Z3 ha impostato x[i][j] a 1
                if (m.eval(x[i][j]).get_numeral_int() == 1) {

                    int nodo_partenza = nodi_dispari[i];
                    int nodo_destinazione = nodi_dispari[j];

                    cout << "  -> Aggiungo percorso tra " << nodo_partenza << " e " << nodo_destinazione
                         << " (Costo: " << distanze_matrice[i][j] << ")" << endl;

                    // Ricostruiamo il percorso usando i padri salvati da Dijkstra
                    const vector<int>& padri = padri_matrice[i];
                    int attuale = nodo_destinazione;

                    while (attuale != nodo_partenza) {
                        int precedente = padri[attuale];
                        if (precedente == -1) break;

                        // Cerchiamo l'arco originale per copiarne il peso
                        int weight = 0;
                        for(const auto& a : adj[precedente]) {
                            if(a.destinazione == attuale) {
                                weight = a.peso;
                                break;
                            }
                        }

                        // Duplicazione arco
                        adj[precedente].push_back({attuale, weight, -1, true});
                        adj[attuale].push_back({precedente, weight, -1, true});

                        attuale = precedente;
                    }
                }
            }
        }
    } else {
        cout << "ERRORE CRITICO: Z3 non ha trovato soluzione (Grafo sconnesso?)." << endl;
    }

    // Ritorna il grafo
    return adj;
}


