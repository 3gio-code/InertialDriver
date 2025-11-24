#include <iostream>
#include "../include/InertialDriver.h"

// Funzione ausiliaria (helper) produce un dato di misurazione fittizio
Measure test_measure(double seed)
{
    Measure m;
    for (int i = 0; i < 17; ++i)
    {
        // Assegniamo valori specifici che agevolano le procedure per il test
        m.readings[i].yaw_v = seed + i * 0.1;
        m.readings[i].yaw_a = seed + i * 0.01;
        m.readings[i].pitch_v = 0;
        m.readings[i].pitch_a = 0;
        m.readings[i].roll_v = 0;
        m.readings[i].roll_a = 0;
    }
    return m;
}

int main()
{
    std::cout << "***** INIZIO DEL TEST DI INERTIAL DRIVER *****" << std::endl;

    InertialDriver driver;

    std::cout << std::endl
              << "1. Test di verifica per il riempimento Buffer (BUFFER_DIM = " << BUFFER_DIM << ")" << std::endl;
    // Inseriamo dati oltre la soglia del buffer per simulare e controllare l'effetto di sovrascrittura
    for (int i = 1; i <= BUFFER_DIM + 2; ++i)
    {
        std::cout << "Pushing misura " << i << "..." << std::endl;
        Measure m = test_measure(static_cast<double>(i));
        driver.push_back(m);
    }

    std::cout << "L'attuale dimensione (dovrebbe essere " << BUFFER_DIM << "): " << driver.get_current_size() << std::endl;

    std::cout << std::endl
              << "2. Test di verifica della stampa dell'ultima misura" << std::endl;
    /* Ci aspettiamo che il risultato visualizzi il valore 7, a dimostrazione che l'elemento 1 e 2 sono stati
    correttamente ciclicamente sovrascritti*/
    std::cout << driver << std::endl;

    std::cout << std::endl
              << "3. Test di verifica per get_reading" << std::endl;
    // Eseguiamo la verifica del valore acquisito dal sensore 0 più recente (il cui seed e' 7.0)
    try
    {
        Reading r = driver.get_reading(0);
        std::cout << "Sensore 0 ultima misura: " << r.yaw_v << std::endl;
    }
    catch (InertialDriver::BufferEmptyException)
    {
        std::cerr << "Eccezione, buffer vuoto!" << std::endl;
    }

    catch (InertialDriver::SensorIndexOutOfBoundException)
    {
        std::cerr << "Eccezione. indici errati!" << std::endl;
    }

    std::cout << std::endl
              << "4. Test di verifica per pop_front" << std::endl;
    /*Data la capacita' di 5 e gli inserimenti 1, 2, 3, 4, 5, 6, 7, il buffer finale e' [3, 4, 5, 6, 7].
    Il primo elemento da estrarre e' 3*/
    try
    {
        Measure old = driver.pop_front();
        std::cout << "Popped misura: " << old.readings[0].yaw_v << std::endl;

        std::cout << "Dimensione dopo pop (dovrebbe essere " << BUFFER_DIM - 1 << "): " << driver.get_current_size() << std::endl;
    }
    catch (InertialDriver::BufferEmptyException)
    {
        std::cerr << "Eccezione, buffer vuoto!" << std::endl;
    }

    catch (InertialDriver::SensorIndexOutOfBoundException)
    {
        std::cerr << "Eccezione, indici errati!" << std::endl;
    }

    std::cout << std::endl
              << "5. Test di verifica per clear_buffer" << std::endl;
    driver.clear_buffer();
    std::cout << "Dimensione dopo clear: " << driver.get_current_size() << std::endl;

    // Verifica del comportamento in caso di buffer vuoto
    try
    {
        driver.pop_front();
    }
    catch (InertialDriver::BufferEmptyException)
    {
        std::cout << "Eccezione, buffer vuoto!" << std::endl;
    }

    catch (InertialDriver::SensorIndexOutOfBoundException)
    {
        std::cerr << "Eccezione, indici errati!" << std::endl;
    }

    std::cout << "***** TEST TERMINATO *****" << std::endl;
    return 0;
}