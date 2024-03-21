% Zadani c. 40:
% Napiste program resici ukol dany predikatem u40(LIN), kde LIN je vstupni 
% celociselny seznam s nejmene dvema cisly. Predikat je pravdivy (ma hodnotu 
% true), pokud se v seznamu LIN pravidelne stridaji licha a suda cisla, 
% jinak je nepravdivy (ma hodnotu false).

% Testovaci predikaty:
u40_1:- u40([4,-3,2,1,8,3,8,-1]).			% true
u40_2:- u40([4,-3,2,1,8,4,8,-1]).			% false
u40_3:- u40([-3,2]).					% true
u40_r:- write('Zadej LIN: '),read(LIN),u40(LIN).

% Reseni:
u40(LIN):- length(LIN, LINlength), LINlength >= 2, processingList(LIN). % seznam ma alespon 2 polozky -> processingList

processingList([_]). % konec seznamu (pouze 1 prvek)
processingList([X, Y | T]):- current(X, Y), processingList([Y | T]). % vezme ze seznamu 1 par a rekurivne se zavola na zbytek seznamu vcetne 2. hodnoty z paru

currentPair(X, Y):- number(X), number(Y), 0 is X mod 2, 1 is Y mod 2. % kontrola cisel a samotne deleni 2 pro zjisteni sudy/lichy
currentPair(X, Y):- number(X), number(Y), 0 is Y mod 2, 1 is X mod 2. % pripadne lichy/sudy
