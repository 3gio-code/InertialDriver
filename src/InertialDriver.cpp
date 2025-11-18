#include "InertialDriver.h"
#include <stdexcept> // Per std::out_of_range

// COSTRUTTORE
InertialDriver::InertialDriver() : head(0), count(0)
{
    // Inizializziamo il driver. Il MyVector buffer parte vuoto.
}

// IMPLEMENTAZIONE FUNZIONI PRIVATE
int InertialDriver::get_tail_index() const
{
    // La coda (elemento più vecchio) si trova 'count' passi indietro rispetto alla testa, circolarmente.
    // In un buffer circolare standard: tail = (head - count + capacity) % capacity
    if (count == 0)
        return 0;
    return (head - count + BUFFER_DIM) % BUFFER_DIM;
}

// IMPLEMENTAZIONE FUNZIONI PUBBLICHE

// Source: 22
void InertialDriver::push_back(const Measure &m)
{
    // Caso 1: Il buffer fisico del MyVector non è ancora pieno (fase di riempimento iniziale)
    if (buffer.size() < BUFFER_DIM)
    {
        buffer.push_back(m);
        head = (head + 1) % BUFFER_DIM;
        count++;
    }
    // Caso 2: Il buffer fisico è pieno, operiamo sovrascrittura circolare
    else
    {
        buffer.set(m, head);            // Sovrascrive all'indice head corrente
        head = (head + 1) % BUFFER_DIM; // Avanza la testa

        // Se il buffer era già logicamente pieno, count resta BUFFER_DIM.
        // Abbiamo sovrascritto il dato più vecchio, quindi la 'tail' avanza implicitamente.
        if (count < BUFFER_DIM)
        {
            count++;
        }
    }
}

// Source: 23
Measure InertialDriver::pop_front()
{
    if (count == 0)
    {
        // Gestione errore buffer vuoto.
        // Nota: Potresti lanciare un'eccezione o ritornare una misura vuota.
        throw std::runtime_error("Buffer vuoto: impossibile eseguire pop_front");
    }

    int tail = get_tail_index();
    Measure val = buffer.get(tail); // Recupera elemento più vecchio

    // Rimuovere logicamente significa decrementare il conteggio.
    // I dati rimangono nel MyVector ma verranno sovrascritti o ignorati.
    count--;

    return val;
}

// Source: 24
void InertialDriver::clear_buffer()
{
    // Reset logico. Non serve deallocare il MyVector (lo farà il distruttore),
    // basta dire che ci sono 0 elementi validi.
    count = 0;
    head = 0;
    // Possiamo anche svuotare fisicamente il vettore per pulizia,
    // ma reimpostare gli indici è sufficiente per la logica circolare.
    // Se volessimo usare le funzioni di MyVector:
    while (buffer.size() > 0)
    {
        buffer.pop_back();
    }
}

// Source: 25
Reading InertialDriver::get_reading(int index)
{
    if (index < 0 || index > 16)
    {
        throw std::out_of_range("Indice sensore non valido (deve essere 0-16)");
    }
    if (count == 0)
    {
        throw std::runtime_error("Buffer vuoto: nessuna lettura disponibile");
    }

    // "ritorna la corrispondente lettura della misura più recente"
    // La misura più recente è quella appena prima di 'head' (gestendo il wrap-around)
    int newest_index = (head - 1 + BUFFER_DIM) % BUFFER_DIM;

    // Dato che il MyVector potrebbe non essere ancora pieno fisicamente all'inizio:
    // Se count < BUFFER_DIM, l'ultimo elemento inserito è semplicemente buffer[head-1]
    // (senza modulo, o meglio, il modulo funziona se head > 0).

    // Sicurezza aggiuntiva: se il buffer fisico non ha ancora raggiunto quella dimensione, usiamo get()
    // MyVector farà il suo check interno.
    Measure recent_measure = buffer.get(newest_index);

    return recent_measure.readings[index];
}

int InertialDriver::get_current_size() const
{
    return count;
}

// Source: 26 - Overloading operator<<
std::ostream &operator<<(std::ostream &os, InertialDriver &driver)
{
    if (driver.count == 0)
    {
        os << "Nessuna misura nel buffer.";
        return os;
    }

    // Stampa l'ultima misura salvata (la più recente)
    // Riutilizziamo la logica usata in get_reading
    int newest_index = (driver.head - 1 + BUFFER_DIM) % BUFFER_DIM;
    Measure m = driver.buffer.get(newest_index);

    os << "=== ULTIMA MISURA (Indice interno: " << newest_index << ") ===\n";
    for (int i = 0; i < 17; ++i)
    {
        os << "Sensore " << i << ": "
           << "Y(v:" << m.readings[i].yaw_v << ", a:" << m.readings[i].yaw_a << ") "
           << "P(v:" << m.readings[i].pitch_v << ", a:" << m.readings[i].pitch_a << ") "
           << "R(v:" << m.readings[i].roll_v << ", a:" << m.readings[i].roll_a << ")\n";
    }
    return os;
}