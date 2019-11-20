# Laborator 9

Se folosesc functii de baza din *MPI*:

- `MPI_Comm_rank()`
- `MPI_Comm_size()`
- `MPI_Send()`
- `MPI_Recv()`
- `MPI_Bcast()`
- `MPI_Scatter()`
- `MPI_Gather()`
- `MPI_Status()`

## Nota

Toate exercitiile au un numar cu 1 mai mare deacat cel din pdf, deoarece au fost
rezolvate pe varianta veche a acestuia...

## Algoritmul Monte Carlo

Se calculeaza *pi* de catre mai multe procese, folosind *Algoritmul Monte Carlo*.
Astfel, cercul unitate este inscris intr-un patrat. In acest patrat se genereaza
aleator un numar mare (1000000 pentru fiecare proces) de puncte. Se numara cate
dintre acestea sunt in interiorul cercului. Procesul 0 primeste numerele de
puncte gasite in cerc de catre toate celelalte procese si face suma acestora,
impreuna cu numarul gasit de el insusi.

La final, procesul cu rangul 0 va aproxima *pi* dupa urmaotarea formula:

```
arie_cerc = pi * r * r  // he he
arie_patrat = (2 * r) * (2 * r)
=> pi = 4 * arie cerc / arie_patrat ~= numar_puncte_in_cerc / numar_puncte_total
```