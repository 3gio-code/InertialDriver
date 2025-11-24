#ifndef INERTIAL_DRIVER_H
#define INERTIAL_DRIVER_H

#include "MyVector.h"
#include <iostream>

// Costante per la dimensione del buffer (nota a tempo di compilazione)
constexpr int BUFFER_DIM = 5;
// Costante per il numero dei sensori (nota a tempo di compilazione)
constexpr int N_READINGS = 17;

// Rappresenta una lettura
struct Reading
{
    double yaw_v;
    double yaw_a;
    double pitch_v;
    double pitch_a;
    double roll_v;
    double roll_a;
};

// Rappresenta una misura
struct Measure
{
    Reading readings[N_READINGS]; // Array C-style obbligatorio di 17 elementi
};
class InertialDriver
{

public:
    // le due classi che vanno a gestire le eccezioni
    // quando il buffer è vuoto
    class BufferEmptyException
    {
    };
    // errore nei valori degli indici
    class SensorIndexOutOfBoundException
    {
    };

    InertialDriver();

    /**
     * Aggiunge una misura al buffer
     * Sovrascrive la meno recente se il buffer è pieno
     */
    void push_back(const Measure &m);

    /**
     * Restituisce la misura più vecchia e la rimuove dal buffer
     * Lancia eccezione se il buffer è vuoto.
     */
    Measure pop_front();

    // Elimina tutte le misure salvate senza restituirle
    void clear_buffer();

    /**
     * Ritorna la lettura corrispondente all'indice (0-16) della misura PIÙ RECENTE.
     * Non cancella dal buffer
     */
    Reading get_reading(int index);

    int get_current_size() const;

private:
    // Buffer gestito obbligatoriamente con la classe MyVector
    MyVector<Measure> buffer{BUFFER_DIM};

    // Variabili per la gestione della logica a buffer circolare, uso head e tail (varia in base a head e count) e count come riferimenti
    int head;
    int count;

    int get_tail_index() const;
};

// Stampa a schermo l'ultima misura salvata
std::ostream &operator<<(std::ostream &os, InertialDriver &driver);

#endif