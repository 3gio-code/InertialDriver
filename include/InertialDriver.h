#ifndef INERTIAL_DRIVER_H
#define INERTIAL_DRIVER_H

#include "MyVector.h"
#include <iostream>

// Costante per la dimensione del buffer
constexpr int BUFFER_DIM = 5; // Impostato a 5 per facilitare i test  (scelta arbitraria)
// Costante per il numero dei sensori
constexpr int N_READINGS = 17; // Impostata a 17

// DEFINIZIONE STRUTTURE DATI

/**
 * Rappresenta la "Lettura" di un singolo sensore.
 * Contiene 6 valori double.
 */
struct Reading
{
    double yaw_v;
    double yaw_a;
    double pitch_v;
    double pitch_a;
    double roll_v;
    double roll_a;
};

/**
 * Rappresenta la "Misura" completa del sistema.
 * Composta da 17 letture organizzate in un array stile C.
 */
struct Measure
{
    Reading readings[N_READINGS]; // Array C-style obbligatorio di 17 elementi
};

// CLASSE INERTIAL DRIVER

class InertialDriver
{

public:
    // le due classi che mi vanno a cìgestire le eccezioni
    class BufferEmptyException
    {
    }; // quando il buffer è vuoto
    class SensorIndexOutOfBoundException
    {
    }; // errore nei valori degli indici
    // Costruttore
    InertialDriver();

    /**
     * Aggiunge una misura al buffer.
     * Sovrascrive la meno recente se il buffer è pieno
     */
    void push_back(const Measure &m);

    /**
     * Restituisce la misura più vecchia e la rimuove dal buffer
     * Lancia eccezione se il buffer è vuoto.
     */
    Measure pop_front();

    /**
     * Elimina tutte le misure salvate senza restituirle
     */
    void clear_buffer();

    /**
     * Ritorna la lettura corrispondente all'indice (0-16) della misura PIÙ RECENTE.
     * Non cancella dal buffer
     */
    Reading get_reading(int index);

    // Getter per debug/utility (opzionale ma utile per main)
    int get_current_size() const;

private:
    // Buffer gestito obbligatoriamente con la classe MyVector
    MyVector<Measure> buffer{BUFFER_DIM};

    // Variabili per la gestione della logica a buffer circolare
    int head;  // Indice di scrittura (dove inserire il prossimo elemento)
    int count; // Numero attuale di elementi validi nel buffer

    // Funzione helper per calcolare l'indice dell'elemento più vecchio (tail)
    int get_tail_index() const;
};

// Overloading operatore<< (dichiarato friend per accedere ai dati se necessario, o usa getter)
// Stampa a schermo l'ultima misura salvata
std::ostream &operator<<(std::ostream &os, InertialDriver &driver);

#endif // INERTIAL_DRIVER_H