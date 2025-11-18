#ifndef MyVector_H
#define MyVector_H

#include <ostream>

template <typename T>
class MyVector
{

private:
    // Var privata che contiene la size totale di celle di memoria inizializzate del MyVector
    int bueffer_sz;
    // Var privata che contiene la size utilizzata del MyVector
    int sz;
    // Var privata che punta al primo degli elementi del MyVector (in memoria)
    T *elem;
    // Funzione che controlla se l'indice sta nel range di lunghezza del MyVector, termina correttamente se è così, lancia IndexOutOfBoundException altrimenti
    void index_check(int i);
    // Funzione che controlla se il bueffer è pieno, se si chiama resize
    void check_bueffer();
    // Funzione che ingrandisce il bueffer
    void resize();

    // COSTANTI:
    static constexpr int kDefaultBuefferSize = 10;
    static constexpr int kDefaultResizeFactor = 2;
    static constexpr int kDefaultSize = 0;

public:
    // CLASSI ECCEZIONI:

    // Classe eccezione
    class NegativeSizeException
    {
    };
    class IndexOutOfBoundException
    {
    };

    // DISTRUTTORE:
    ~MyVector();

    // COSTRUTTORI:

    // Costruttore di default
    MyVector();

    // Costruttore che accetta un int e lo assegna alla size
    explicit MyVector(int s);

    // Costruttore con initializer_list
    // lst è passato per copia
    MyVector(std::initializer_list<T> lst);

    // Costruttore di copia
    MyVector(const MyVector &arg);

    // Costruttore di MOVE (assegna ed invalida)
    MyVector(MyVector &&arg);

    // OPERATORI

    // Overloading operatore COPIA (permette l'assegnamento della copia)
    MyVector<T> &operator=(const MyVector<T> &v);

    // Overloading operatore MOVE (assegna ed invalida)
    MyVector<T> &operator=(MyVector<T> &&);

    // Overloading operatore[]
    T &operator[](int n);

    // Overlaoding operatore[] const (SOLO LETTURA perchè returna COPIA non reference)
    // Posso fare overloading di una funzione in base alla sua CONSTNESS
    T operator[](int n) const;

    // SETTERS/GETTERS:

    // Funzione che returna la size del MyVector
    int size() const;

    // Funzione che returna la bueffer_size del MyVector
    int bueffer_size() const;

    // Funzione che setta un elemento del MyVector
    void set(T d, int index);

    // Funzione che returna un elemento del MyVector
    T get(int index);

    // FUNCTIONS:

    // Funzione che rimuove l'ultimo elemento e lo returna (ACCESSO SOLO IN LETTURA)
    T pop_back();

    // Funzione che aggiunge un elemento alla fine
    void push_back(T el);

    // Funzione che returna una reference all'elemento in posizione index, lancia eccezioni se indice non valido
    T &at(int index);

    // Funzione che fa si che bueffer_sz sia almeno grande quanto n
    void reserve(int n);
};

// OVERLOADING OPERATORI
// Overloading operatore<< (stream)
template <typename T>
std::ostream &operator<<(std::ostream &os, MyVector<T> a);

#include "MyVector.hpp"

#endif // MyVector_H