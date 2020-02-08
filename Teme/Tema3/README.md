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

Acest comportament se poate observa in exemplele de rulare date mai jos.

## Scalare
S-a incercat minimizarea numarului de mesaje interschimbate de procese si
deblocarea cat mai rapida in momentul in care trebuie ca acestea sa-si trimita
liniile de inceput si final. Astfel, ruland scriptul `test_scalability.sh` pe un
**Intel i7-8700K @ 4.2Ghz** se pot observa urmatoarele:

Pentru evaluarea algoritmului se vor folosi cele mai mari poze disponibile:
landscape.pnm (3840x2160) si rorshcach.pgm (3853x2000).

Pentru landscape.pnm se obtin urmatorii timpi:
```
teo@obor Tema3 $ ./test_scalability.sh input_files/PNM/landscape.pnm landscape_smooth.pnm 200 12
rm -f tema3 *.o .fuse_hidden* *.pgm *.pnm

mpicc -Wall -Wextra -Wpedantic tema3.c -c
mpicc -Wall -Wextra -Wpedantic pnm_image_utils.c -c
mpicc -Wall -Wextra -Wpedantic filters.c -c
mpicc -Wall -Wextra -Wpedantic image_processing.c -c
mpicc tema3.o pnm_image_utils.o filters.o image_processing.o -o tema3 -lm

Testare cu NP = 1
153.11user 0.12system 2:33.53elapsed 99%CPU (0avgtext+0avgdata 59660maxresident)k
48608inputs+16408outputs (4major+17379minor)pagefaults 0swaps

Testare cu NP = 2
156.81user 0.10system 1:18.72elapsed 199%CPU (0avgtext+0avgdata 36632maxresident)k
0inputs+17344outputs (12major+22018minor)pagefaults 0swaps

Testare cu NP = 3
152.62user 0.12system 0:51.20elapsed 298%CPU (0avgtext+0avgdata 36664maxresident)k
0inputs+17680outputs (15major+24423minor)pagefaults 0swaps

Testare cu NP = 4
152.44user 0.15system 0:38.43elapsed 397%CPU (0avgtext+0avgdata 37040maxresident)k
0inputs+20144outputs (24major+26600minor)pagefaults 0swaps

Testare cu NP = 5
152.68user 0.16system 0:30.85elapsed 495%CPU (0avgtext+0avgdata 36992maxresident)k
0inputs+21072outputs (34major+28344minor)pagefaults 0swaps

Testare cu NP = 6
152.86user 0.17system 0:25.79elapsed 593%CPU (0avgtext+0avgdata 37276maxresident)k
0inputs+22000outputs (36major+30072minor)pagefaults 0swaps

Testare cu NP = 7
200.05user 11.51system 0:30.53elapsed 692%CPU (0avgtext+0avgdata 37140maxresident)k
0inputs+22928outputs (46major+31545minor)pagefaults 0swaps

Testare cu NP = 8
206.07user 8.43system 0:27.13elapsed 790%CPU (0avgtext+0avgdata 37332maxresident)k
0inputs+23864outputs (51major+33192minor)pagefaults 0swaps

Testare cu NP = 9
205.54user 5.46system 0:23.77elapsed 887%CPU (0avgtext+0avgdata 37380maxresident)k
0inputs+24792outputs (60major+34800minor)pagefaults 0swaps

Testare cu NP = 10
207.43user 3.40system 0:21.40elapsed 984%CPU (0avgtext+0avgdata 37532maxresident)k
0inputs+25760outputs (64major+36328minor)pagefaults 0swaps

Testare cu NP = 11
211.03user 2.61system 0:19.75elapsed 1081%CPU (0avgtext+0avgdata 37536maxresident)k
0inputs+26648outputs (77major+37814minor)pagefaults 0swaps

Testare cu NP = 12
220.69user 2.52system 0:18.94elapsed 1178%CPU (0avgtext+0avgdata 37504maxresident)k
0inputs+27624outputs (81major+39342minor)pagefaults 0swaps
```

Pentru rorschach.pgm se obtin urmatorii timpi:
```
teo@obor Tema3 $ ./test_scalability.sh input_files/PGM/rorschach.pgm rorschach_smooth.pgm 200 12
rm -f tema3 *.o .fuse_hidden* *.pgm *.pnm

mpicc -Wall -Wextra -Wpedantic tema3.c -c
mpicc -Wall -Wextra -Wpedantic pnm_image_utils.c -c
mpicc -Wall -Wextra -Wpedantic filters.c -c
mpicc -Wall -Wextra -Wpedantic image_processing.c -c
mpicc tema3.o pnm_image_utils.o filters.o image_processing.o -o tema3 -lm

Testare cu NP = 1
44.33user 0.04system 0:44.55elapsed 99%CPU (0avgtext+0avgdata 26100maxresident)k
15056inputs+16408outputs (4major+8980minor)pagefaults 0swaps

Testare cu NP = 2
44.46user 0.08system 0:22.47elapsed 198%CPU (0avgtext+0avgdata 24872maxresident)k
24inputs+17200outputs (12major+11490minor)pagefaults 0swaps

Testare cu NP = 3
44.63user 0.06system 0:15.10elapsed 296%CPU (0avgtext+0avgdata 24820maxresident)k
0inputs+17576outputs (14major+13206minor)pagefaults 0swaps

Testare cu NP = 4
44.94user 0.09system 0:11.46elapsed 392%CPU (0avgtext+0avgdata 25152maxresident)k
0inputs+20032outputs (24major+15019minor)pagefaults 0swaps

Testare cu NP = 5
45.33user 0.10system 0:09.30elapsed 488%CPU (0avgtext+0avgdata 24936maxresident)k
0inputs+20928outputs (33major+16568minor)pagefaults 0swaps

Testare cu NP = 6
45.38user 0.14system 0:07.86elapsed 579%CPU (0avgtext+0avgdata 24796maxresident)k
0inputs+21824outputs (35major+18139minor)pagefaults 0swaps

Testare cu NP = 7
60.73user 4.02system 0:09.50elapsed 681%CPU (0avgtext+0avgdata 24856maxresident)k
0inputs+22720outputs (48major+19548minor)pagefaults 0swaps

Testare cu NP = 8
61.37user 2.78system 0:08.28elapsed 774%CPU (0avgtext+0avgdata 25176maxresident)k
0inputs+23624outputs (52major+21047minor)pagefaults 0swaps

Testare cu NP = 9
61.77user 1.98system 0:07.34elapsed 868%CPU (0avgtext+0avgdata 24856maxresident)k
0inputs+24520outputs (63major+22581minor)pagefaults 0swaps

Testare cu NP = 10
63.06user 1.65system 0:06.73elapsed 960%CPU (0avgtext+0avgdata 24976maxresident)k
0inputs+25416outputs (67major+23973minor)pagefaults 0swaps

Testare cu NP = 11
63.33user 1.02system 0:06.10elapsed 1053%CPU (0avgtext+0avgdata 24936maxresident)k
0inputs+26352outputs (69major+25519minor)pagefaults 0swaps

Testare cu NP = 12
63.87user 0.72system 0:05.66elapsed 1140%CPU (0avgtext+0avgdata 24992maxresident)k
0inputs+27896outputs (76major+27021minor)pagefaults 0swaps
```

Ruland scriptul de mai multe ori, media timpilor obtinuti este similara timpilor
din rularile de mai sus.

### Concluzie
Se observa deci o scalare aproape ideala de la 1 la 6 procese (numarul de nuclee
fizice ale procesorului), insa pe masura ce numarul proceselor creste,
dimensiunile partilor din imagine gestionate de fiecare dintre acestea scad, in
timp ce dimensiunea mesajelor pe care procesele le schimba intre ele inainte sa
aplice filtrele ramane constanta. Pe deasupra, atunci cand se executa mai mult
de un proces pe un nucleu, se pierde localitatea spatiala a datelor din cacheul
respectivului nucleu, ceea ce va genera multe *missuri*.

Prin urmare, necesitatea sincronizarilor mentionate mai sus, precum si timpii
mari de cautare in memorie vor crea o latenta din ce in ce mai mare relativ la
dimensiunea datelor fiecarui proces, motiv pentru care timpii de rulare scad sub
valorile celor pentru 6 procese, abia cand se folosesc 9 procese.