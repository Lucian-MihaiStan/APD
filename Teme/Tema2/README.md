<<<<<<< HEAD
# APD
Algoritmi Paraleli si Distribuiti - UPB 2019-2020

Contine laboratoare, teme si testul practic de la final

Temele vor fi incarcate pe *Git* **dupa deadline**

## Laboratoare
1. **POSIX**: Introducere in `pthread.h`
2. **POSIX**: *mutex* si *bariera*
3. **POSIX**: Inmultire de matrice executata in paralel + [Algoritmul lui Strassen](https://en.wikipedia.org/wiki/Strassen_algorithm)
4. **POSIX**: Sortari paralelizate: *merge sort*, [OETS](https://en.wikipedia.org/wiki/Odd%E2%80%93even_sort), *shear sort*
5. **Java**: Introducere in API-ul de multithreading al *Java*
6. **Java**: *Producatori - Consumatori*, *Problema filosofilor* si cautare binara
7. **Java**: Structuri de date (*thread-safe*) sincronizate
8. **Java**: *Executor Service* folosit pentru a rezolva probleme clasice de *backtracking*
9. **MPI**: Introducere in *MPI*
10. **MPI**: Cateva functii ale *MPI* implementate de mana, pipeline folosit
pentru sortare si calculare de polinom + rank sort
11. **MPI**: `MPI_Isend()`, `MPI_Irecv()` si algoritmi pentru:
- stabilirea topologiei
- gasirea liderului
- numararea statiilor dintr-un cluster

## Teme
1. **POSIX**: *DFT* si *FFT*
2. **Java**: Explorare de graf prin calculare de hashuri
3. **MPI**: Procesare de imagini prin aplicare de filtre

## Test
Colocviul final - inmultire paralela de matrice in *Java*
=======
# Tema 2
Se descopera un graf in paralel. Pentru a parcurge o muchie si a vizita un nou
nod este nevoie sa se hashuiasca de mai multe ori un string corespunzator
respectivei muchii.

Entitatile care exploreaza graful se numesc **exploratori**, iar cei care
verifica hashurile calculate de acestia si transmit nodurile ce trebuie vizitate
se numesc **HQ**.

## Message
Reprezinta mesajul efectiv trimis fie de catre un explorator la HQ sau invers.

**Nota:** singurul motiv pentru care tema asta prezinta o oarecare dificultate e
faptul ca HQ-urile trimit 2 mesaje pentru fiecare muchie pe modelul:

```
parinte
copil frecventa
```
Asadar, canalul de comunicatie se ocupa de concatenarea acestora, mesajele ce
ajung la exploratori continand ambele mesaje de mai sus intr-unul singur.

## Communication Channel
Contine 2 liste de mesaje: una de la exploratori la HQ-uri si alta invers.
Din acest motiv, urmand modelul *Producatori - consumatori* cu precizarea ca
acum nu mai exista o limita pentru dimensiunea bufferelor, este nevoie de
semafoare pentru a marca faptul ca bufferul arondat semaforului nu este gol
(pentru a putea scoate elemente din el). Cele 2 liste sunt sincronizate folosind
`Collections.synchronizedList`.

Cum am mentionat si mai sus, cand un explorator pune un mesaj in buffer, acesta
este adaugat ca atare. In schimb, cand un HQ vrea sa transmita un mesaj,
canalul de comunicare va scrie in mesaj si ultimul parinte trimis, pe langa
nodul copil si mesajul trimis deja de HQ.

## Space Explorer
Toti exploratorii folosesc un *ConcurrentHashMap* partajat (folosit ca *Set*)
pentru a retine nodurile explorate. Cand un  nod citeste un mesaj de la HQ, 
verifica daca nodul copil este deja vizitat. In caz contrar, il marcheaza ca
vizitat, ii decodifica frecventa si o trimite catre HQ pentru validare. In cazul
in care nodul copil este deja vizitat, se trece la urmatorul mesaj.

## HQ
Trimite vecinii unui nod deja vizitat prin mecanismul descris mai sus si
verifica frecventele primite de la exploratori. Cand toata galaxia e explorata,
trimite cate un mesaj cu stringul `"EXIT"` pentru fiecare explorator astfel
incat fiecare va sti sa se opreasca.
>>>>>>> Tema2-APD/master
