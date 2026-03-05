/*ESEMPIO 1
DATA BASE*/
arco(a, b, 10).
arco(b, c, 5).
arco(c, d, 8).
arco(d, e, 12).
arco(e, c, 10).
arco(c, a, 7).
/*PROCEDURE*/

ppc(NodoIniziale,Cammino,CostoTotale):-  
 findall(arco(X,Y,W),arco(X,Y,W),Archi),  
 cammina(NodoIniziale,NodoIniziale,Archi,Cammino,0,CostoTotale),!.
 
cammina(NodoCorrente,NodoCorrente, [], [NodoCorrente], CostoAccumulativo, CostoAccumulativo).

cammina(NodoAttuale,NodoCorrente, ArchiDisponibili, [NodoAttuale|CamminoRestante], CostoAccumulativo, CostoTotale) :-
    select(ArcoGenerico, ArchiDisponibili, ArchiRimasti),
    connesso(NodoAttuale, ArcoGenerico, NodoSuccessivo, Peso),
	NuovoCosto is CostoAccumulativo + Peso,
    cammina(NodoSuccessivo, NodoCorrente,ArchiRimasti, CamminoRestante, NuovoCosto, CostoTotale).

connesso(X,arco(X,Y,W),Y,W).

connesso(X,arco(Y,X,W),Y,W).
