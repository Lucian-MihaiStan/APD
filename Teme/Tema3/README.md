# Tema3-APD
Procesare de imagini folosind *MPI*

## Structura de fisiere
Se vor explica functionalitatile fiecarui header.

### general_utils
Contine doar defineuri pentru `min()`, `max()`, proceul lider (`MASTER` - 0),
precum si un `DIE()` usor modificat denumit `ASSERT()`.

### filters
Expune doar functia `getFilter()` care va popula filtrul dat ca parametru cu
valorile corespunzatoare numelui acestuia (primit tot ca parametru). In cazul
in care numele nu este unul cunoscut, filtrul oferit va fi cel unitate.

### pnm_image_utils
Dupa cum spune si numele, expune functii utilitare folosite la manipularea
imaginilor in format `.pgm` si `.pnm`:
	- citire
	- scriere
	- aplicarea unui filtru

Prin intermediul macroului `ASSERT` explicat mai sus, se vor verifica diversele
cazuri de eroare ce pot aparea (cele care nu sunt verificate sunt specificate
in comentariile aferente fiecarei functii).

### image_processing
Singura functie non-statica, `processImage()`, va calcula pentru fiecare proces
ce linii ii revin acestuia pentru prelucrare, dupa care va aplica efectiv
filtrele, urmand ca la final sa recompuna toata imaginea in procesul `MASTER`
care va aduna fragmentele fiecarui alt proces si le va concatena fragmentului
sau.

## Algoritm
Fiecare proces are de prelucrat (prin aplicarea tuturor filtrelor) un anumit
numar de linii din imagine. Pentru un paralelism mai bun, fiecare proces va
citi doar portiunea sa din imagine. De asemenea, pentru a nu fi afectate prea
mult de latenta introdusa de transmiterea multor mesaje, datele sunt retinute
in forma liniarizata, ca un vector.

Inainte sa inceapa aplicarea unui filtru, un proces `p` va primi prima linie
(bordura pe care nu o va procesa efectiv) de la procesul `p - 1` si pe ultima
de la `p + 1`. Exceptie fac procesele `0` si `NP - 1` care nu vor primi prima si
respectiv ultima linie.

Aplicarea convolutiei se face "cupland" un anumit element din matrice cu
corespondentul sau din filtru.

### Nota
Datorita liniarizarii imaginii sub forma unui vector, pentru imagini cu latimi
(exprimate ca numar de octeti) mai mici decat lungimea liniei de cache / 2
algoritmul va functiona mai repede decat in cazul unei implementari clasice,
deoarece toate cele 9 elemente pe care se va aplica operatia de convolutie vor
putea fi stocate in cacheul procesorului.

Acest comportament se poate observa in exemplul de rulare dat mai jos.

## Scalare
S-a incercat minimizarea numarului de mesaje interschimbate de procese si
deblocarea cat mai rapida in momentul in care trebuie ca acestea sa-si trimita
liniile de inceput si final. Astfel, ruland scriptul `test_scaling.sh` pe un
**Intel i7-8700K @ 4.2Ghz** se pot observa urmatoarele:

Pentru evaluarea algoritmului se va folosi cea mai mare poza disponibila:
landscape.pnm (3840x2160).

```
teo@obor Tema3 $ ./test_scaling.sh input_files/PNM/landscape.pnm landscape_smooth.pnm
rm -f tema3 *.o .fuse_hidden* *.pgm *.pnm

mpicc -Wall -Wextra -Wpedantic tema3.c -c
mpicc -Wall -Wextra -Wpedantic pnm_image_utils.c -c
mpicc -Wall -Wextra -Wpedantic filters.c -c
mpicc -Wall -Wextra -Wpedantic image_processing.c -c
mpicc tema3.o pnm_image_utils.o filters.o image_processing.o -o tema3 -lm

Testare cu NP = 1
152.58user 0.13system 2:33.08elapsed 99%CPU (0avgtext+0avgdata 59628maxresident)k
72848inputs+16408outputs (118major+17380minor)pagefaults 0swaps

Testare cu NP = 2
152.68user 0.08system 1:16.64elapsed 199%CPU (0avgtext+0avgdata 36612maxresident)k
24inputs+17392outputs (12major+22042minor)pagefaults 0swaps

Testare cu NP = 3
157.17user 0.09system 0:52.73elapsed 298%CPU (0avgtext+0avgdata 36492maxresident)k
0inputs+17656outputs (14major+24416minor)pagefaults 0swaps

Testare cu NP = 4
154.86user 0.13system 0:39.04elapsed 396%CPU (0avgtext+0avgdata 37004maxresident)k
0inputs+20144outputs (24major+26597minor)pagefaults 0swaps

Testare cu NP = 5
156.82user 0.23system 0:31.71elapsed 495%CPU (0avgtext+0avgdata 37000maxresident)k
0inputs+21216outputs (31major+28404minor)pagefaults 0swaps

Testare cu NP = 6
160.62user 0.23system 0:27.18elapsed 591%CPU (0avgtext+0avgdata 37100maxresident)k
40inputs+22000outputs (40major+30076minor)pagefaults 0swaps

Testare cu NP = 7
200.20user 10.26system 0:30.47elapsed 690%CPU (0avgtext+0avgdata 37172maxresident)k
0inputs+22720outputs (49major+31544minor)pagefaults 0swaps

Testare cu NP = 8
203.28user 7.34system 0:26.67elapsed 789%CPU (0avgtext+0avgdata 37300maxresident)k
0inputs+24104outputs (55major+33207minor)pagefaults 0swaps

Testare cu NP = 9
207.72user 5.15system 0:24.08elapsed 883%CPU (0avgtext+0avgdata 37312maxresident)k
8inputs+24792outputs (61major+34803minor)pagefaults 0swaps

Testare cu NP = 10
210.79user 3.85system 0:21.92elapsed 978%CPU (0avgtext+0avgdata 37448maxresident)k
0inputs+25720outputs (61major+36295minor)pagefaults 0swaps

Testare cu NP = 11
217.53user 3.74system 0:20.65elapsed 1071%CPU (0avgtext+0avgdata 37576maxresident)k
0inputs+26648outputs (67major+37823minor)pagefaults 0swaps

Testare cu NP = 12
238.75user 8.42system 0:21.68elapsed 1139%CPU (0avgtext+0avgdata 37504maxresident)k
0inputs+27584outputs (62major+39261minor)pagefaults 0swaps
```

Se observa deci o scalare aproape ideala de la 1 la 6 procese, insa pe masura
ce numarul proceselor creste, dimensiunile partilor din imagine gestionate de
fiecare dintre acestea scad, in timp ce dimensiunea mesajelor pe care procesele
le schimba intre ele inainte sa aplice filtrele ramane constanta. Prin urmare,
necesitatea acestor sincronizari va crea o latenta din ce in ce mai mare relativ
la dimensiunea datelor fiecarui proces.