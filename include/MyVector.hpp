#ifndef MyVector_HPP
#define MyVector_HPP
#include <iostream>
#include <algorithm>

#ifdef __INTELLISENSE__
#include "MyVector.h"
#endif

// FUNZIONI PRIVATE:

// Funzione che controlla se l'indice sta nel range di lunghezza del MyVector, termina correttamente se è così, lancia IndexOutOfBoundException altrimenti
template <typename T>
void MyVector<T>::index_check(int i)
{
	if (i < 0 || i > sz)
		throw IndexOutOfBoundException();
}

// Funzione che controlla se il buffer è pieno, se si chiama resize
template <typename T>
void MyVector<T>::check_buffer()
{
	if (buffer_size == sz)
		resize();
}

// Funzione che ingrandisce il buffer
template <typename T>
void MyVector<T>::resize()
{
	buffer_size *= kDefaultResizeFactor; // buffer_size = buffer_size*2;
	T *temp = elem;						 // creo un puntatore temporaneo alle celle di memoria di elem
	elem = new T[buffer_size];			 // ora faccio puntare elem a buffer_size nuove celle di memoria
	std::copy(temp, temp + sz, elem);	 // effettuo copia dei vecchi elementi in elem (ingrandito)
	delete[] temp;						 // cancello il "vecchio elem" (le vecchie celle di memoria assegnate a elem)
	// invalido temp
	temp = nullptr;
}

// FUNZIONI PUBBLICHE:

// DISTRUTTORE:
template <typename T>
MyVector<T>::~MyVector()
{
	delete[] elem;
	// delete sz; IMPLICITO
	// delete buffer_size; IMPLICITO
	// se aggiungo la seguente riga vedo che alla fine del programma la funzione distruttore viene chiamata automaticamente
	// std::cout<<"deleted"<< std::endl;
}
// COSTRUTTORI:

// Costruttore di default
template <typename T>
MyVector<T>::MyVector() : sz{kDefaultSize}, elem{nullptr} {}

// Costruttore che accetta un int e lo assegna alla size
template <typename T>
MyVector<T>::MyVector(int s)
{
	if (s < 0)
		throw NegativeSizeException();
	sz = s;
	// il MyVector ha una buffer_size minima di 10
	if (s < kDefaultbufferSize)
		s = kDefaultbufferSize;
	buffer_size = s;
	elem = new T[s];
}

// Costruttore con initializer_list
// lst è passato per copia
template <typename T>
MyVector<T>::MyVector(std::initializer_list<T> lst)
{
	sz = (int)lst.size();
	// il MyVector ha una buffer_size minima di 10
	int s = sz;
	if (s < kDefaultbufferSize)
		s = kDefaultbufferSize;
	buffer_size = s;
	elem = new T[s];
	std::copy(lst.begin(), lst.end(), elem); // effettuo copia degli elementi di lst in elem
}

// Costruttore di COPIA
template <typename T>
MyVector<T>::MyVector(const MyVector<T> &arg) : sz{arg.sz}, buffer_size{arg.buffer_size}, elem{new T[arg.buffer_size]}
{ // accesso diretto ai membri privati di arg (possibile perchè oggetto di classe MyVector)
	std::copy(arg.elem, arg.elem + sz, elem);
}

// Costruttore di MOVE (assegna ed invalida)
template <typename T>
MyVector<T>::MyVector(MyVector &&arg) : sz{arg.sz}, buffer_size{arg.buffer_size}, elem{arg.elem}
{ // copia size, buffer_size e puntatore
	// invalido arg
	arg.sz = 0;
	arg.buffer_size = 0;
	arg.elem = nullptr; // invalido puntatore di arg così che in fase di distruzione non venga eliminato il puntatorecon gli elementi "passati"
}

// OPERATORI:

// Overloading operatore COPIA (permette l'assegnamento della copia)
template <typename T>
MyVector<T> &MyVector<T>::operator=(const MyVector<T> &arg)
{
	T *p = new T[arg.buffer_size];			   // alloco arg.buffer_size nuove celle
	std::copy(arg.elem, arg.elem + arg.sz, p); // copio il contenuto di arg in arg.sz delle nuove celle
	delete[] elem;							   // cancello i vecchi dati di elem
	elem = p;								   // faccio puntare il vettore al nuovo spazio riempito
	sz = arg.sz;							   // il MyVector ora ha la sz del nuovo spazio riempito
	buffer_size = arg.buffer_size;			   // il MyVector ora ha la buffer_size del nuovo spazio riempito

	return *this; // return self-reference
}

// Overloading operatore MOVE (assegna ed invalida)
template <typename T>
MyVector<T> &MyVector<T>::operator=(MyVector<T> &&arg)
{
	delete[] elem;				   // cancello i vecchi elementi a cui puntava il mio vettore
	elem = arg.elem;			   // lo faccio puntare ai nuovi elementi, quelli di arg
	sz = arg.sz;				   // aggiorno anche la sua sz
	buffer_size = arg.buffer_size; // aggiorno anche la sua buffer_size
	// invalido arg
	arg.elem = nullptr;
	arg.sz = 0;
	arg.buffer_size = 0;
	return *this; // return self-reference
}

// Overloading operatore[]
template <typename T>
T &MyVector<T>::operator[](int n)
{ // accesso sia in lettura che in scrittura (perchè ho usato la reference), ma NON funziona se deve returnare oggetti const
	index_check(n);
	return elem[n];
}

// Overlaoding operatore[] const (SOLO LETTURA perchè returna COPIA non reference)
// Posso fare overloading di una funzione in base alla sua CONSTNESS
template <typename T>
T MyVector<T>::operator[](int n) const
{ // accesso solo in lettura (xk ho usato CONST)
	index_check(n);
	return elem[n];
}

// SETTERS/GETTERS:

template <typename T>
int MyVector<T>::size() const
{
	return sz;
}

template <typename T>
int MyVector<T>::buffer_size() const
{
	return buffer_size;
}

template <typename T>
void MyVector<T>::set(T d, int index)
{
	index_check(index);
	elem[index] = d;
}
template <typename T>
T MyVector<T>::get(int index)
{
	index_check(index);
	return elem[index];
}

// FUNCTIONS:

// Funzione che rimuove l'ultimo elemento e lo returna (ACCESSO SOLO IN LETTURA)
template <typename T>
T MyVector<T>::pop_back()
{						   // NB: NON uso la REFERENCE, xk non ha senso voler SCRIVERE su una CELLA DI MEMORIA AZZERATA
	T temp = elem[sz - 1]; // copio l'ultimo elemento del MyVector
	--sz;				   // azzero l'ultimo elemento del My Vector e decremento la sz di 1
	return temp;
}

// Funzione che aggiunge un elemento alla fine
template <typename T>
void MyVector<T>::push_back(T el)
{
	check_buffer();
	elem[sz++] = el;
}

// Funzione che returna una reference all'elemento in posizione index, lancia eccezioni se indice non valido
template <typename T>
T &MyVector<T>::at(int index)
{
	index_check(index);
	return elem[index];
}

// Funzione che fa si che buffer_size sia almeno grande quanto n
template <typename T>
void MyVector<T>::reserve(int n)
{
	for (int i = 0; (buffer_size - sz) < n; i++) // faccio tanti resize, quanti sono sufficienti a rendere falsa (buffer_size-sz)<n
		resize();								 // non mi interessa se ad ogni resize raddoppio la buffer_size e rischio di avere "più spazio disponibile"
												 // poichè non è richiesto che lo spazio disponibile sia esattamente n, ma basta che sia >n
}
// OVERLOADING OPERATORI

// Overloading operatore<< (stream)
template <typename T>
std::ostream &operator<<(std::ostream &os, MyVector<T> v)
{
	std::string output = "[ ";
	for (int i = 0; i < v.size(); i++)
	{
		output += std::to_string(v.get(i));
		output += " , ";
	}
	output = output.substr(0, output.length() - 2) + ']';
	return os << output;
}
#endif // MyVector_HPP