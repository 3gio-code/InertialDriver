//MyVector NON modificato dalle lezioni precedenti
#ifndef MyVector_H
#define MyVector_H

#include <iostream>

template <typename T>
class MyVector
{

    //dichiarazioni PUBBLIC 
public:
    // CLASSI ECCEZIONI:

    // Eccezione per termine negativo
    class NegativeSizeException {};
    // Eccezione per indice fuori dai bound
    class IndexOutOfBoundException {};

    // DISTRUTTORE:
    ~MyVector();

    // COSTRUTTORI:

    // Costruttore default
    MyVector();

    // Costruttore che accetta un int 
    explicit MyVector(int s);

    // Costruttore che accetta un initializer_list
    MyVector(std::initializer_list<T> lst);

    // Costruttore di copia
    MyVector(const MyVector &arg);

    // Costruttore che assegna e invalida
    MyVector(MyVector &&arg);

    // OPERATORI:

    // Operator[]
    T &operator[](int n);

    // Overlaoding operator[] const 
    // solamente lettura perchè returna una copia e non reference
    T operator[](int n) const;

    // Operator =
    MyVector<T> &operator=(const MyVector<T> &v);

    // Overloading operator = 
    MyVector<T> &operator=(MyVector<T> &&);

    // GETTERS/SETTERS:

    // Funzione che returna size 
    int size() const;

    // Funzione che returna buffer_size 
    int get_buffer_size() const;

    // Funzione che getta un elemento del MyVector
    T get(int index);

    // Funzione che setta un elemento del MyVector
    void set(T x, int index);

    // FUNZIONI:

    // Funzione che rimuove l'ultimo elemento 
    T pop_back();

    // Funzione che aggiunge un elemento alla fine
    void push_back(T el);

    // Funzione che returna una all'indice passato
    T &at(int index);

    // Funzione che fa si che buffer_size sia almeno grande quanto n
    void reserve(int n);

    //dichiarazioni PRIVATE
    private:
    // Variabile che contiene la dimensione totale disponibile in MyVector
    int buffer_size;
    // Variabile che contiene la dimensioine effettiva di MyVector
    int sz;
    // Variabile "puntatore" che punta al primo degli elementi del MyVector 
    T *elem;
    // Funzione helper che fa un check per gli indici
    void index_check(int i);
    // Funzione helper che guarda la dimensione
    void check_buffer();
    // Funzione resize classica
    void resize();

    // COSTANTI di valore arbitrario:
    static constexpr int defaultbufferSize = 10;
    static constexpr int defaultResizeFactor = 2;
    static constexpr int defaultSize = 0;

};

// operatore<< 
//dichiarato fuori dal main
template <typename T>
std::ostream &operator<<(std::ostream &os, MyVector<T> a);

#include "MyVector.hpp"

#endif // MyVector_H