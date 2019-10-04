# Laborator 2

Utilizare **mutex**, **barrier** si rezolvarea unui *race condition*.

Toate cerintele pot fi rulate si exemplificate prin regulile `run_cerinta` din *Makefile*.

## allCores

Se executa `sqrt(ULONG_MAX)` de `10000^3` ori pentru a tine toate coreurile procesorului la 100%.

## addVectors

Se aduna 2 vectori folosind paralelism.

Fiecare *thread* efectueaza adunarea unei portiuni din vectori.

## raceCondition

Rezolva un *race condition* creat de 2 threaduri care executa ambele `a += 2` asupra aceleiasi variabile
globale folosind un **mutex**.

Pentru verificare se poate folosi scriptul `testCorrectnessIntensive.sh`.

## oneTwo

Afiseaza 1 si 2 de 1000 de ori fiecare. Fiecare afisare se face intr-un thread separat.
Se doreste sa se afiseze mai intai 1 de 1000 de ori si apoi 2.

Pentru sincronizare se foloseste o **bariera**.