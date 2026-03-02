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
