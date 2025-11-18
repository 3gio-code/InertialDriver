#include <iostream>
#include "InertialDriver.h"

// Funzione helper per creare una misura fittizia
Measure create_dummy_measure(double seed) {
    Measure m;
    for (int i = 0; i < 17; ++i) {
        // Riempiamo con valori riconoscibili per il test
        m.readings[i].yaw_v = seed + i * 0.1;
        m.readings[i].yaw_a = seed + i * 0.01;
        m.readings[i].pitch_v = 0;
        m.readings[i].pitch_a = 0;
        m.readings[i].roll_v = 0;
        m.readings[i].roll_a = 0;
    }
    return m;
}

int main() {
    std::cout << "--- INIZIO TEST INERTIAL DRIVER ---" << std::endl;

    InertialDriver driver;

    std::cout << "\n1. Test Riempimento Buffer (BUFFER_DIM = " << BUFFER_DIM << ")" << std::endl;
    // Inseriamo più elementi della dimensione del buffer per testare la sovrascrittura
    for (int i = 1; i <= BUFFER_DIM + 2; ++i) {
        std::cout << "Pushing misura " << i << "..." << std::endl;
        Measure m = create_dummy_measure((double)i);
        driver.push_back(m);
    }

    std::cout << "Dimensione attuale (dovrebbe essere " << BUFFER_DIM << "): " << driver.get_current_size() << std::endl;

    std::cout << "\n2. Test Stampa Ultima Misura (operator<<)" << std::endl;
    // Dovrebbe stampare la misura 7 (poiché abbiamo inserito 7 elementi e sovrascritto 1 e 2)
    std::cout << driver << std::endl;

    std::cout << "\n3. Test get_reading" << std::endl;
    // Testiamo la lettura del sensore 0 dell'ultima misura (che ha seed 7.0)
    try {
        Reading r = driver.get_reading(0);
        std::cout << "Sensore 0 ultima misura (Atteso yaw_v ~ 7.0): " << r.yaw_v << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Errore: " << e.what() << std::endl;
    }

    std::cout << "\n4. Test pop_front (FIFO)" << std::endl;
    // Poiché abbiamo inserito 1, 2, 3, 4, 5, 6, 7 in un buffer da 5:
    // Il buffer contiene [3, 4, 5, 6, 7]. Il più vecchio è 3.
    try {
        Measure old = driver.pop_front();
        std::cout << "Popped misura (Atteso seed 3.0 per sensore 0): " << old.readings[0].yaw_v << std::endl;
        
        std::cout << "Dimensione dopo pop (dovrebbe essere " << BUFFER_DIM - 1 << "): " << driver.get_current_size() << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Errore: " << e.what() << std::endl;
    }

    std::cout << "\n5. Test clear_buffer" << std::endl;
    driver.clear_buffer();
    std::cout << "Dimensione dopo clear: " << driver.get_current_size() << std::endl;

    // Test errore su buffer vuoto
    try {
        driver.pop_front();
    } catch (const std::exception& e) {
        std::cout << "Eccezione catturata correttamente su pop vuoto: " << e.what() << std::endl;
    }

    std::cout << "\n--- FINE TEST ---" << std::endl;
    return 0;
}