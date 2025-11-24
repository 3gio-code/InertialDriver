#include "../include/InertialDriver.h"
#include <iostream>

InertialDriver::InertialDriver() : head(0), count(0)
{
}

// IMPLEMENTAZIONE FUNZIONI PRIVATE
int InertialDriver::get_tail_index() const
{
    // L'elemento piu vecchio si trova 'count' passi indietro rispetto alla testa.
    // In un buffer circolare standard: tail = (head - count + capacity) % capacity
    if (count == 0)
        return 0;
    return (head - count + BUFFER_DIM) % BUFFER_DIM;
}

void InertialDriver::push_back(const Measure &m)
{
    // buffer non pieno
    if (buffer.size() < BUFFER_DIM)
    {
        buffer.push_back(m);
        head = (head + 1) % BUFFER_DIM;
        count++;
    }
    // buffer pieno
    else
    {
        buffer.set(m, head);
        head = (head + 1) % BUFFER_DIM;

        if (count < BUFFER_DIM)
        {
            count++;
        }
    }
}

Measure InertialDriver::pop_front()
{
    // buffer vuoto
    if (count == 0)
    {
        throw BufferEmptyException();
    }

    int tail = get_tail_index();
    Measure val = buffer.get(tail);

    count--;

    return val;
}

void InertialDriver::clear_buffer()
{
    count = 0;
    head = 0;
}

Reading InertialDriver::get_reading(int index)
{
    if (index < 0 || index >= N_READINGS)
    {
        throw SensorIndexOutOfBoundException();
    }
    if (count == 0)
    {
        throw BufferEmptyException();
    }

    int new_index = (head - 1 + BUFFER_DIM) % BUFFER_DIM;

    Measure recent_measure = buffer.get(new_index);

    return recent_measure.readings[index];
}

int InertialDriver::get_current_size() const
{
    return count;
}

std::ostream &operator<<(std::ostream &os, InertialDriver &driver)
{
    if (driver.get_current_size() == 0)
    {
        os << "Nessuna misura nel buffer.";
        return os;
    }

    os << "=== ULTIMA MISURA SALVATA ===\n";

    for (int i = 0; i < N_READINGS; ++i)
    {
        try
        {
            Reading r = driver.get_reading(i);

            os << "Sensore " << i << ": "
               << "Y(" << r.yaw_v << ", " << r.yaw_a << ") "
               << "P(" << r.pitch_v << ", " << r.pitch_a << ") "
               << "R(" << r.roll_v << ", " << r.roll_a << ")\n";
        }
        catch (InertialDriver::SensorIndexOutOfBoundException())
        {
            os << "Sensore " << i << ": Errore lettura\n";
        }
    }

    return os;
}