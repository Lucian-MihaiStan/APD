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
