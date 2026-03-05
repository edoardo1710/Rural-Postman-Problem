# Rural Postman Problem (C++) 📮
Questo repository contiene l'implementazione in C++ di un risolutore per il **Rural Postman Problem (RPP)**.

Il progetto è in fase di sviluppo come elaborato finale per l'esame del corso di "**Computabilità, Complessità e Logica**" presso l'Università degli Studi di Trieste.

## 👥 Autori
* **Bottosso Edoardo Antonio**
* **Giacomin Paolo**

## Introduzione e Definizione del Problema

Per capire meglio il Rural Postman Problem bisogna innanzitutto introdurre il **Chinese Postman Problem**.

Un **Circuito Euleriano** è un circuito che visita tutti gli archi una e una sola volta e di conseguenza una **rete euleriana** è una rete che ammette un circuito euleriano al suo interno.

Concentriamoci sulle reti non orientate; una rete non orientata è euleriana se e solo se è connessa e ogni nodo ha grado pari.

Per trovare un Circuito Euleriano si utilizza l'algoritmo di **EndParing**:

1. si trovano i sottocircuiti che non condividono archi

2. partendo dal primo nodo, si segue il sottocircuito fino a che non si trova un nodo condiviso e si cambia percorso

A questo punto ci è possibile introdurre il Chinese Postman Problem.

Sia $G=(N, A)$ un grafo tale per cui $\forall\,i, j\in A,\,c_{ij}\geq 0$, ovvero il peso di ogni arco è maggiore di $0$: l'obiettivo è quello di trovare un circuito che visita tutti gli archi almeno una volta a costo minimo.

Se la rete è euleriana si utilizza l'algoritmo di EndPairing, mentre se è non euleriana la si rende tale tramite il **Matching Ottimo**, ovvero rendere i nodi dispari pari aggiungendo il minor numero possibile di archi.

Nel caso di reti non orientate, trovata la soluzione del problema iniziale, l'arco verrà percorso più volte; alla fine, il costo totale del percorso sarà dato da

$$
z = \sum_{(i,j) \in A} c_{ij} + c
$$

dove $c$ rappresenta il numero di archi ripetuti.

Il Rural Postman Problem è un'estensione del Chinese Postman Problem in cui al posto di visitare tutti gli archi se ne visita solo un sottoinsieme. 

Sia $G=(N, A)$ un grafo tale per cui $\forall\,i, j\in A,\,c_{ij}\geq 0$ e sia $R\subset A$ l'insieme degli archi da visitare. Sia $G_R$ la rete degli archi contenuti in $R$: e la rete $G_R$ è connessa ed euleriana allora basta utilizzare l'algoritmo di End-Pairing, mentre se non è euleriana la si rende tale usando gli archi di $A\setminus R$.

La differenza fondamentale tra il Chinese Postman Problem e il Rural Postman Problem sta proprio nella **complessità**:

- nel Chinese Postman Problem se il grafo è già euleriano basta sommare il peso di ogni arco, mentre se non è euleriano basta applicare il Matching Ottimo, che ha complessità polinomiale. Poiché il matching ottimo si risolve in tempo polinomiale, l'intero problema è polinomiale.

- nel Rural Postman Problem, oltre al Matching Ottimo, bisogna anche connettere tra di loro le componenti isolate del grafo. Se immaginiamo che ogni componente connessa di $R$ sia un singolo "super-nodo", connetterli con il costo minimo per formare un ciclo è strutturalmente identico a risolvere il **Problema del Commesso Viaggiatore**, che è notoriamente NP-Hard.

 ## Approccio algoritmico

 ### Algoritmo di Dijkstra

 Il file ```dijkstra.cpp``` contiene l'implementazione dell'algoritmo di Dijkstra. 

 Vengono inizializzati due vettori, ```dist``` e ```parent```, che conterranno rispettivamente le distanze di ciascun nodo dalla sorgente e il predecessore di ciascun nodo all'interno dell cammino minimo. Per analizzare i nodi da esplorare viene utilizzata una **min-heap**, in modo tale da rendere la complessità dell'algoritmo $O\left((V+E) \cdot \log(V)\right)$.

 ### Algoritmo di connessione

 Il file ```connessione.cpp``` contiene l'implementazione di un algoritmo per connettere diverse componenti connesse di un grafo cercando di minimizzare il costo totale dei collegamenti tramite Z3.

 Inizialmente vengono individuate le componenti, che verranno trattate come dei "super-nodi", dopodiché viene riempita una matrice ```macro_grafo[K][K]``` che contiene la distanza minima tra l'isola $i$ e l'isola $j$. Contemporaneamente, ```best_bridge``` salva quali sono i due nodi reali che permettono quel collegamento minimo.

 Z3 viene inizializzato in questo modo:

 - **Variabili decisionali ```vector<vector<expr>> x, f```**: $x_{i, j}$, booleana che indica se il collegamento tra l'isola $i$ e l'isola $j$ viene attivato, e $f_{i, j}$, che funge da variabile di supporto per capire se le componenti sono connesse.

 - **Vincoli**: conservazione del flusso, garantisce che non esistano isole isolate o cicli separati, simmetria, per grafi non orientati, e costo, che viene minimizzato

Successivamente, si identificano quali $x_{i,j}$ sono stati impostati a $1$ dal modello e viene ricostruito il cammino tramite il vettore dei predecessori fornito da Dijkstra.

 La complessità totale è $O\left(K^2\cdot (V+E) \cdot \log(V) + \text{costo SMT solver}\right)$.

 ### Algoritmo per il Matching Ottimo

 Il file ```matchingOttimo.cpp``` contiene l'implementazione di un algoritmo per effettuare il Matching Ottimo tramite Z3.

 Inizialmente, l'algoritmo identifica i nodi dispari e dopodiché viene calcolata la distanza tra qualsiasi coppia di nodi dispari tramite l'algoritmo di Dijkstra.

  Z3 viene inizializzato in questo modo:

   - **Variabile decisionale ```vector<vector<expr>> x```**: $x_{i, j}$, booleana, che se uguale a $1$ indica che il nodo dispari $i$ è accoppiato con il nodo dispari $j$.

   - **Vincoli**: ogni nodo deve essere accoppiato con esattamente un altro nodo dispari e Z3 cerca la combinazione di coppie che minimizza la somma dei pesi.

 Dopo aver deciso le coppie di nodi migliori il codice recupera il cammino minimo reale tra i due nodi usando ```padri_matrice``` e per ogni arco che compone quel cammino, viene aggiunta una copia dell'arco nella lista di adiacenza ```adj```, marcato come necessario.

 La complessità totale è $O\left(K\cdot (V+E) \cdot \log(V) + \text{costo SMT solver}\right)$.
 
 ### Algoritmo di EndPairing

 Il file ```EndPairing.cpp``` contiene l'implementazione dell'**Algoritmo di Hierholzer** per l'EndPairing. 
 
L'algoritmo segue una logica simile a quella che adotta l'algoritmo **Depth-First Search (DFS)**. All'interno del ciclo ```while```, il codice estrae l'ultimo arco disponibile per il nodo corrente ```u``` utilizzando ```adj[u].back()``` e lo rimuove immediatamente con ```pop_back()```, in modo tale da evitare eventuali cicli infiniti sullo stesso arco.

L'algoritmo applica due controlli:

- saltare gli archi non necessari tramite ```!next.necessario```

- evita di ripercorrere un arco già utilizzato in una chiamata precedente tramite ```archi_visitati[next.id]```

Quando l'algoritmo trova un nodo che non ha archi da visistare lo aggiunge a ```circuito``` e viene aggiornato il peso del percorso.

La complessità temporale è $O\left(V+E\right)$ e quella spaziale è $O\left(V+E\right)$.

## Dettagli implementativi

Per la risoluzione delle problematiche di ottimizzazione vincolata (come il Matching Ottimo e la connessione delle componenti), il progetto adotta la libreria esterna **Z3 Theorem Prover** di Microsoft Research. Tutte le definizioni delle strutture dati e le firme delle funzioni sono centralizzate nel file ```header.h```. La compilazione del sistema è gestita tramite un Makefile dedicato: utilizzando il comando ```make postino.o```, verrà generato l'eseguibile a partire dal file ```main.cpp```, garantendo un processo di build rapido e riproducibile.

