# Laborator 4
Se paralelizeaza diverse tipuri de sortari:
- _shear sort_
- _merge sort_
- _bubble sort_

Toate rularile si testarile se pot efectua prin intermediuk fisierului
`Makefile`.

## Merge sort
Initial fiecare thread va opera pe intervalul sau.

Cand diferenta dintre indicii de start si de sfarsit este egala cu lungimea
intervalului de interclasare, threadurile se vor suprapune 2 cate 2. In acest
caz, unul moare iar celalat se extinde pentru a-i incorpora intervalul.

Inversarea vectorilor (cel de interclasare si cel de date) se face de catre
threadul 0.

## Bubble sort
Se implementeaza **OETS**.

Se foloseste o bariera in plus pentru a rula algoritmul pana cand sirul este
sortat, nu de un numar fix de `N` ori.

## Shear sort
Pentru a se putea folosi (oarecum) `qsort` si pe coloane, se transpune matricea
inainte de sortarea pe coloane si apoi se transpune din nou matricea dupa
`qsort`,