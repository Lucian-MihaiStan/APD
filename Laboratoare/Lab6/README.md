# Laborator 6
*Producer - Consumer*, *Dining philosophers* si cautare binara

## Producer - Consumer
Se implementeaza patternul *Producer - Consumer* atat pentru un consumator si
un producator cat si pentru consumatori si producatori multipli.

Bufferul are de fiecare data o capacitate de 1 elemen.

Se folosesc **semafoare** pentru a se respecta constrangerile problemei:
- nu se fac citiri si scrieri in buffer in acelasi timp;
- nu se fac citiri concomitent;
- nu se fac scrieri concomitent 

## Dining philosophers
Se rezolva problema filosofilor. In timp ce ceilalti filosofi dau lock mai intai
pe stanga si apoi pe dreapta, unul dintre acestia functioneaza invers, reusind
sa termine de mancat si eliberandu-si lockurile. In continuare, ceilalti
filosofi vor proceda ca la incput, acum fiind destule resurse disponibile pentru
toti.

## Cautare binara
Cautare binara paralelizata: fiecare thread cauta intr-un subinterval din cel
de interes. La fiecare pas, intervalul de interes, devine intervalul threadului
intre capetele caruia se afla elementul cautat.

Complexitatea este `O(logp(n))`.