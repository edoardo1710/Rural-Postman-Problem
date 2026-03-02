/*ESEMPIO 2
DATA BASE*/
arco(a, b, 10).
arco(b, c, 5).
arco(c, d, 8).
arco(d, e, 12).
arco(e, c, 10).
arco(c, a, 7).
/*PROCEDURE*/

:- dynamic elemento_temporaneo/1. 

/*serve per fare in modo che assertz e retract non diano errore*/

trovaArchi(Template, Goal, ListaFinale) :-
    (   call(Goal),
        assertz(elemento_temporaneo(Template)),
        fail;
        true
    ),
    raccogliElementi(ListaFinale).


raccogliElementi([Elemento | Resto]) :-
    retract(elemento_temporaneo(Elemento)), !, 
    raccogliElementi(Resto).
raccogliElementi([]).


ppc(NodoIniziale,Cammino,CostoTotale):-  
 trovaArchi(arco(X,Y,W),arco(X,Y,W),Archi),  
 cammina(NodoIniziale,NodoIniziale,Archi,Cammino,0,CostoTotale),!.
 
cammina(NodoCorrente,NodoCorrente, [], [NodoCorrente], CostoAccumulativo, CostoAccumulativo).

cammina(NodoAttuale,NodoCorrente, ArchiDisponibili, [NodoAttuale|CamminoRestante], CostoAccumulativo, CostoTotale) :-
    select(ArcoGenerico, ArchiDisponibili, ArchiRimasti),
    connesso(NodoAttuale, ArcoGenerico, NodoSuccessivo, Peso),
	NuovoCosto is CostoAccumulativo + Peso,
    cammina(NodoSuccessivo, NodoCorrente,ArchiRimasti, CamminoRestante, NuovoCosto, CostoTotale).

connesso(X,arco(X,Y,W),Y,W).
connesso(X,arco(Y,X,W),Y,W).