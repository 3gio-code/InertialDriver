# InertialDriver

PROGETTO: Inertial Sensor Driver
MEMBRI DEL GRUPPO:

- Francesco Birtele
- Giovanni Faggin
- Leonardo Dall Mura

---

## DESCRIZIONE DELLE ATTIVITÀ E SUDDIVISIONE DEL LAVORO

FASE PRELIMINARE (Tutto il gruppo)
Il lavoro è iniziato con una fase congiunta di analisi e progettazione della durata
di due giorni. In questa fase, il gruppo ha discusso e definito:

1. L'architettura logica della classe InertialDriver.
2. Le specifiche delle strutture dati (Reading e Measure) in relazione ai requisiti
   del buffer circolare.
3. La strategia di integrazione con la classe MyVector.

SCELTE PROGETTUALI
È stato deciso di utilizzare la classe template MyVector (sviluppata nelle
esercitazioni precedenti) importandola senza alcuna modifica al suo codice sorgente.
Questa scelta è stata dettata dalla volontà di rispettare il principio di singola
responsabilità e di riuso del codice: MyVector agisce puramente come contenitore
di memoria dinamica, mentre la logica di gestione "circolare" (indici di testa/coda,
sovrascrittura) è stata interamente incapsulata all'interno della classe
InertialDriver.

SUDDIVISIONE DELLO SVILUPPO

Francesco e Giovanni:

- Implementazione della classe InertialDriver (file .h e .cpp).
- Definizione delle struct di supporto e gestione della logica degli indici
  per il buffer circolare.
- Implementazione dei metodi di interfaccia (push, pop, get) e gestione
  delle eccezioni.

Leonardo:

- Sviluppo del file main.cpp per la verifica delle funzionalità.
- Stesura dei test case
- Organizzazione della struttura del progetto (cartelle src/include) e gestione
  della compilazione e del linking tra i vari file.

---
