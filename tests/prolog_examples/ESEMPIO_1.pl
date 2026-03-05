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
 cammina(NodoIniziale,Archi,Cammino,0,CostoTotale). 
cammina(NodoCorrente, [], [NodoCorrente], CostoAccumulativo, CostoAccumulativo).
cammina(NodoCorrente, ArchiDisponibili, [NodoCorrente|CamminoRestante], CostoAccumulativo, CostoTotale) :-
    select(arco(NodoCorrente, NodoSuccessivo, Peso), ArchiDisponibili, ArchiRimasti),
    NuovoCosto is CostoAccumulativo + Peso,
    cammina(NodoSuccessivo, ArchiRimasti, CamminoRestante, NuovoCosto, CostoTotale).
