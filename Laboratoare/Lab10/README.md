# Laborator 10
Se implementeaza de mana `MPI_Bcast()`, `MPI_Scan()`, `MPI_Reduce()`,
`MPI_Bcast()`, precum si calcularea unui polinom si sortare folosind pipeline
si prin *rank sort*.

## Bcast
Transmite o valoare catre toate procesele in timp logaritmic. Din momentul in
care un proces isi primeste numarul, acesta va incepe sa il transmita mai
departe altor procese.

La fiecare pas, `i`, un proces cu rangul `r` care a primit deja numarul, il va
trimite catre procesul `r + (1 << i)`.

## Reduce
Calculeaza suma elementelor unui vector. Initial, se foloseste *MPI_Scatter*
pentru ca fiecare proces sa aiba o portiune din vector pe care va calcula
sume partiale in mod clasic, secvential.

Apoi, similar cu [merge sort](https://github.com/teodutu/APD/blob/master/Laboratoare/Lab4/mergeSort.c),
fiecare al doilea proces va trimite suma sa catre procesul din stanga si apoi nu
va mai executa nimic.

La final, procesul 0 are suma totala.

## Scan
Se calculeaza sumele partiale ale unui vector. Se face pe dos fata de *Reduce*:
procesele trimit sumele lor la dreapta in loc de stanga si fiecare proces retine
suma elementelor de pana la el, inclusiv.

Logica dupa care se face trimitera sumelor partiale de la un proces la altul este
similara celei din *Reduce*.

## Poly
Se calculeaza un polinom folosind notiunea de *pipeline*. Fiecare proces este
responsabil de calcularea polinomului pana la gradul reprezentat de rangul sau `r`
si de calcularea lui `x ^ r`. Apoi, procesul trimite suma sa urmatorului proces,
care va adauga `ar+1 * x ^ (r + 1)` la respectiva suma s.a.m.d.

Pentru eficienta, fiecare proces cu rang `r` va trimite mai departe 3 valori:
`x`, `x ^ r` si suma pana la respectivul proces.

Ultimul proces afiseaza rezultatul calculului.

## Sort
Sorteaza un vector prin pipeline. Fiecare proces stocheaza un numar. Primul
proces genereaza numere aleatoare.

Cand primeste un nou numar, fiecare proces il compara cu ce avea inainte. Daca
avea un numar mai mare, pastreaza noul numar si-l da pe cel vechi urmatorului
proces. In caz contrar, da noul numar mai departe.

Un proces cu rang `r` stie ca a terminat de primit numere cand a primit `N - r`
numere, unde `N` este numarul total de numere generat de procesul cu rang 0.

## RankSort
Sorteaza un vector prin algoritmul *rank sort*. Fiecare proces primeste prin
*broadcast* tot vectorul la inceput si este responsabil pentru calcularea
pozitiilor unei anumite parti din vector (interval calculat ca la threaduri).
Pozitiile sunt salvate in vectorul `pos`.

La final, toate procesele ii trimit primului vectorul lor `pos`, iar acesta
pune intr-un vector auxiliar (pentru simplitate) valorile din vel initial
conform pozitiilor primite.