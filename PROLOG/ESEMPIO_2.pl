/*ESEMPIO 3
DATA BASE*/
arco(a, b, 0).
arco(a, c, 0).
arco(a, d, 0).
arco(b, c, 0).
arco(b, d, 0).
arco(c, b, 0).
arco(d, a, 0).
arco(d, b, 0).
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