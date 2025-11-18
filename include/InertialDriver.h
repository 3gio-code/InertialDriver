#ifndef INERTIAL_DRIVER_H
#define INERTIAL_DRIVER_H

#include "MyVector.h"
#include <iostream>

// Costante per la dimensione del buffer (Source: 19)
const int BUFFER_DIM = 5; // Impostato a 5 per facilitare i test, modificabile

// DEFINIZIONE STRUTTURE DATI

/**
 * Rappresenta la "Lettura" di un singolo sensore.
 * Contiene 6 valori double come specificato (Source: 7, 8).
 */
struct Reading {
    double yaw_v;
    double yaw_a;
    double pitch_v;
    double pitch_a;
    double roll_v;
    double roll_a;
};

/**
 * Rappresenta la "Misura" completa del sistema.
 * Composta da 17 letture organizzate in un array stile C (Source: 17).
 * Usiamo una struct wrapper per poter gestire l'array nei return e nel MyVector.
 */
struct Measure {
    Reading readings[17]; // Array C-style obbligatorio di 17 elementi
};

// CLASSE INERTIAL DRIVER

class InertialDriver {
private:
    // Buffer gestito obbligatoriamente con la classe MyVector (Source: 18)
    MyVector<Measure> buffer;

    // Variabili per la gestione della logica a buffer circolare
    int head;    // Indice di scrittura (dove inserire il prossimo elemento)
    int count;   // Numero attuale di elementi validi nel buffer
    
    // Funzione helper per calcolare l'indice dell'elemento più vecchio (tail)
    int get_tail_index() const;

public:
    // Costruttore
    InertialDriver();

    /**
     * Aggiunge una misura al buffer.
     * Sovrascrive la meno recente se il buffer è pieno (Source: 22).
     */
    void push_back(const Measure& m);

    /**
     * Restituisce la misura più vecchia e la rimuove dal buffer (Source: 23).
     * Lancia eccezione se il buffer è vuoto.
     */
    Measure pop_front();

    /**
     * Elimina tutte le misure salvate senza restituirle (Source: 24).
     */
    void clear_buffer();

    /**
     * Ritorna la lettura corrispondente all'indice (0-16) della misura PIÙ RECENTE.
     * Non cancella dal buffer (Source: 25).
     */
    Reading get_reading(int index);

    // Getter per debug/utility (opzionale ma utile per main)
    int get_current_size() const;

    // Overloading operatore<< (dichiarato friend per accedere ai dati se necessario, o usa getter)
    // Stampa a schermo l'ultima misura salvata (Source: 26)
    friend std::ostream& operator<<(std::ostream& os, InertialDriver& driver);
};

#endif // INERTIAL_DRIVER_H