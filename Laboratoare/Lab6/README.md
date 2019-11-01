# Laborator 6
*Producer - Consumer*, *Dining philosophers* si cautare binara

## Producer - Consumer
Se implementeaza patternul *Producer - Consumer* atat pentru un consumator si
un producator cat si pentru consumatori si producatori multipli.

Se folosesc **semafoare** pentru a se respecta constrangerile problemei:
- nu se fac citiri si scrieri in buffer in acelasi timp;
- nu se fac citiri concomitent;
- nu se fac scrieri concomitent 

## Dining philosophers

### Abordare cu timer random
Inainte de a incerca sa ia primul lock, fiecare filosof va astepta un timp
aleator. Astfel, va exista cel putin un filosof care va obtine ambele lockuri
inaintea celorlalti si le va elibera pentru ca si ceilalti sa le poata obtine.

Este mai lent decat abordarea ulterioara din cauza `sleep()`-ului initial, dar
cativa filosofi pot manca in paralel.

### Abordare cu ordinea lockurilor schimbata pentru un filosof
In timp ce ceilalti filosofi dau lock mai intai
pe stanga si apoi pe dreapta, unul dintre acestia functioneaza invers, reusind
sa termine de mancat si eliberandu-si lockurile. In continuare, ceilalti
filosofi vor proceda ca la incput, acum fiind destule resurse disponibile pentru
toti.

Este mai rapida decat abordarea anterioara, dar filosofii mananca secvential.

### Abordare cu oridine de lock schimbata in functie de paritatea indecsilor
Filosofii cu index par vor da lock mai intai pe dreapta si apoi pe stanga, iar
cei cu index impar invers. Astfel, primii mananca in paralel, dupa care mananca
si ultimii tot in paralel.

Pare cea mai eficienta abordare.

## Cautare binara
Cautare binara paralelizata: fiecare thread cauta intr-un subinterval din cel
de interes. La fiecare pas, intervalul de interes, devine intervalul threadului
intre capetele caruia se afla elementul cautat.

Complexitatea este `O(logp(n))`.
