# Laborator 7
Structuri de date sincronizate in *Java*

## Bug Concurrent HashMap
Daca verificarea existentei unei chei si scrierea unei valori corespunzatoare
acesteia nu se executa atomic, apare un race condition la scrierea in *HashMap*.

**Solutia** este sa se foloseasca `putIfAbsent()`, care este o metoda
sincronizata. Doar daca aceasta metoda returneaza `null` (exista deja un element
cu cheia respectiva) se va folosi `put()`-ul clasic.

## lock
Se implementeaza un mutex folosind **AtomicInteger**. Neinteresant.

## Producatori - Consumatori
Se rezolva aceeasi problema a producatorilor si consumatorilor din
[laboratorul 6](https://github.com/teodutu/APD/tree/master/Laboratoare/Lab6/src/multipleProducersMultipleConsumers)
dar folosind un **ArrayBlockingQueue** pe post de buffer. Metodele `put()` si
`take()` din acesta garanteaza sinconizarea.

## Arbore paralel
2 threaduri citesc nodurile unui arbore din fisiere sub forma de perechi
`parinte - fiu` si le introduc in acesta. Un al 3-lea thread valideaza arborele.
Toate threadurile pornesc simultan si se foloseste o bariera (**CyclicBarrier**)
pentru a garanta inserarea tuturor elementelor in arbore inainte de verificare.

De asemenea, metoda `addChild()` a arborelui este sincronizata pentru a nu se
suprascrie noduri.

## Synchronization Problem
Se pleaca de la un race condition clasica: 2 threaduri vor sa incrementeze aceeasi
variabil. Aceasta se rezolva facand respectiva variabila un **AtomicInteger**.
Pentru incrementare se apeleaza metoda `getAndSet()` a acestei clase.

## Lista sincronizata
In mod similar cu problema arborelui paralel, 3 threaduri citesc numere din
fisiere si le insereaza intr-o lista sincronizata (implementata folosind
`Collections.synchronizedList`). Dupa ce s-au scris toate elementele in lista,
un al 4-lea thread sorteaza lista.

Pentru sincronizare nu se mai foloseste o bariera ca in problema precedenta,
ci un semafor initializat la `-2` pe care fiecare thread care scrie il
incrementeaza, iar cel care sorteaza il decrementeaza.
