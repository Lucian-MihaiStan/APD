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
Singura functie non-statica, `processImage()` va calcula pentru fiecare proces
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
153.14user 0.09system 2:33.62elapsed 99%CPU (0avgtext+pfff0avgdata 59716maxresident)k
72848inputs+16408outputs (118major+17381minor)pagefaults 0swaps

Testare cu NP = 2
153.24user 0.13system 1:16.95elapsed 199%CPU (0avgtext+0avgdata 36584maxresident)k
24inputs+17408outputs (12major+22032minor)pagefaults 0swaps

Testare cu NP = 3
155.57user 0.12system 0:52.18elapsed 298%CPU (0avgtext+0avgdata 36656maxresident)k
0inputs+17736outputs (16major+24440minor)pagefaults 0swaps

Testare cu NP = 4
154.81user 0.16system 0:39.02elapsed 397%CPU (0avgtext+0avgdata 36908maxresident)k
0inputs+20256outputs (22major+26621minor)pagefaults 0swaps

Testare cu NP = 5
156.17user 0.20system 0:31.56elapsed 495%CPU (0avgtext+0avgdata 37092maxresident)k
0inputs+21072outputs (28major+28399minor)pagefaults 0swaps

Testare cu NP = 6
160.18user 0.15system 0:27.01elapsed 593%CPU (0avgtext+0avgdata 37076maxresident)k
0inputs+22000outputs (39major+30093minor)pagefaults 0swaps

Testare cu NP = 7
199.68user 10.54system 0:30.34elapsed 692%CPU (0avgtext+0avgdata 37164maxresident)k
0inputs+22928outputs (46major+31626minor)pagefaults 0swaps

Testare cu NP = 8
204.24user 7.73system 0:26.84elapsed 789%CPU (0avgtext+0avgdata 37336maxresident)k
0inputs+24104outputs (54major+33253minor)pagefaults 0swaps

Testare cu NP = 9
207.37user 5.31system 0:24.03elapsed 884%CPU (0avgtext+0avgdata 37352maxresident)k
0inputs+24792outputs (57major+34741minor)pagefaults 0swaps

Testare cu NP = 10
209.79user 3.26system 0:21.71elapsed 980%CPU (0avgtext+0avgdata 37344maxresident)k
0inputs+25720outputs (45major+36290minor)pagefaults 0swaps

Testare cu NP = 11
228.53user 6.42system 0:22.23elapsed 1056%CPU (0avgtext+0avgdata 37384maxresident)k
0inputs+26688outputs (62major+37726minor)pagefaults 0swaps

Testare cu NP = 12
255.27user 13.21system 0:24.05elapsed 1116%CPU (0avgtext+0avgdata 37504maxresident)k
0inputs+27624outputs (57major+39288minor)pagefaults 0swaps
```

Se observa deci o scalare aproape ideala de la 1 la 6 procese, insa pe masura
ce numarul proceselor creste, dimensiunile partilor din imagine gestionate de
fiecare dintre acestea scad, in timp ce dimensiunea mesajelor pe care procesele
le schimba intre ele pe masura ce aplica filtrele ramane constanta. Prin urmare,
necesitatea acestor sincronizari va crea o latenta din ce in ce mai mare relativ
la dimensiunea datelor fiecarui proces.