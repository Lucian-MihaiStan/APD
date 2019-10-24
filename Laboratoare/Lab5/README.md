# Laborator 5
Introducere in multithreading folosind _Java_

Se prezinta concepte de baza precum:
- `Thread`
- `Runnable`
- `synchronized`
- `volatile`

## Buguri
Se dau niste snippeturi de cod care contin fie buguri, fie comportamente aparent
stranii care trebuie reparate, respectiv explicate

### Bug 1
In `main()` se apeleaza metoda `run()` din `MyThread`, ceea ce inseamna ca toate
"threadurile" ruleaza secvential. Pentru a rula in paralel, este nevoie sa se
apeleze `start()`, iar incrementarea lui `value` sa fie **sincronizata**.

### Bug 2
Cand un thread detine lockul unui obiect, orice noua incercare a aceluiasi
thread de a obtine respectivul lock este posibila si nu apare un deadlock.

Fenomenul se numeste [reentrant synchronization](https://docs.oracle.com/javase/tutorial/essential/concurrency/locksync.html).

Pe deasupra, orice clasa are in mod automat lock pe ea insasi.

### Bug 3
Pentru ca a si b sunt initial referinte catre acelasi sir si _Java_ e cretin,
le face aceeasi referinta. Daca se schimba unul dintre siruri, apar
_race conditionuri_ pentru ca threadurile vor fi sincronizate dupa obiecte
diferite.

### Bug 4
Din cauza optimizarilor automate pe care le face _Java_, mofificarea variabilei
`keepRunning` este cunoscuta doar de threadul principal, nu si de cel secundar.
Modificandu-i tipul la unul `volatile`, variabila nu se cacheuieste, iar
modificarile sunt intotdeauna scrise in _RAM_, de unde sunt vizibile pentru
toate threadurile.

### Bug 5
Scenariu clasic de deadlock: fiecare thread obtine un lock si il asteapta pe
celalalt sa-l elibereze pe cel de care are nevoie in continuare.

Solutia este ca cele 2 blocuri `synchronized` sa nu mai fie imbricate, pentru
ca primul bloc din fiecare threa sa-si elibereze lockul inainte de a incerca sa
obtina unul nou.

## doubleVectorElements
```matlab
v .*= 2;
```
paralelizat

## helloWorld
Ca la [laboratorul 1](https://github.com/teodutu/APD/blob/master/Laboratoare/Lab1/helloWorld.c), dar in _Java_.

## shortestPathsFloyd_Warshall
_Roy-Floyd_ paralelizat. Nimic special nici aici.