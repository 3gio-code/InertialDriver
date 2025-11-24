#ifndef MyVector_HPP
#define MyVector_HPP
#include <iostream>

#ifdef __INTELLISENSE__
#include "MyVector.h"
#endif

// Funzione ausiliare per controllare se l'indice sta nel range 
template <typename T>
void MyVector<T>::index_check(int i)
{
	if (i < 0 || i > sz)
		throw IndexOutOfBoundException(); //gestione delle eccezioni con le classi private in MyVector.h
}

// Funzione ausiliare per controllare il buffer quando è pieno
template <typename T>
void MyVector<T>::check_buffer()
{
	if (buffer_size == sz)
		resize();
}

// Classico Resize visto anche a fondamenti
template <typename T>
void MyVector<T>::resize()
{
	buffer_size *= defaultResizeFactor; 
	T *temp = elem;						 
	elem = new T[buffer_size];			 
	std::copy(temp, temp + sz, elem);	 
	delete[] temp;						 
	// invalido temp
	temp = nullptr;
}

//PUBLIC:

// DISTRUTTORE:
template <typename T>
MyVector<T>::~MyVector()
{
	delete[] elem;
}
// COSTRUTTORI:

// Costruttore default
template <typename T>
MyVector<T>::MyVector() : sz{defaultSize}, elem{nullptr} {}

// Costruttore che accetta un int 
template <typename T>
MyVector<T>::MyVector(int s)
{
	if (s < 0)
		throw NegativeSizeException();
	sz = s;

	if (s < defaultbufferSize)
		s = defaultbufferSize;
	buffer_size = s;
	elem = new T[s];
}

// Costruttore che accetta un initializer_list
template <typename T>
MyVector<T>::MyVector(std::initializer_list<T> lst)
{
	sz = (int)lst.size();
	int s = sz;
	if (s < defaultbufferSize)
		s = defaultbufferSize;
	buffer_size = s;
	elem = new T[s];
	std::copy(lst.begin(), lst.end(), elem); 
}

// Costruttore di copia
template <typename T>
MyVector<T>::MyVector(const MyVector<T> &arg) : sz{arg.sz}, buffer_size{arg.buffer_size}, elem{new T[arg.buffer_size]}
{ 
	std::copy(arg.elem, arg.elem + sz, elem);
}

// Costruttore che assegna ed invalida
template <typename T>
MyVector<T>::MyVector(MyVector &&arg) : sz{arg.sz}, buffer_size{arg.buffer_size}, elem{arg.elem}
{ 
	arg.sz = 0;
	arg.buffer_size = 0;
	arg.elem = nullptr; 
}
// OPERATORI:

// Operator[]
template <typename T>
T &MyVector<T>::operator[](int n)
{ 
	index_check(n);
	return elem[n];
}

// Overlaoding operator[] 
template <typename T>
T MyVector<T>::operator[](int n) const
{ // accesso solo in lettura (xk ho usato CONST)
	index_check(n);
	return elem[n];
}

// Operator =
template <typename T>
MyVector<T> &MyVector<T>::operator=(const MyVector<T> &arg)
{
	T *p = new T[arg.buffer_size];			   
	std::copy(arg.elem, arg.elem + arg.sz, p); 
	delete[] elem;							   
	elem = p;								   
	sz = arg.sz;							   
	buffer_size = arg.buffer_size;			   

	return *this; 
}

// Overloading operator =
template <typename T>
MyVector<T> &MyVector<T>::operator=(MyVector<T> &&arg)
{
	delete[] elem;				   
	elem = arg.elem;			   
	sz = arg.sz;				  
	buffer_size = arg.buffer_size; 
	arg.elem = nullptr;  
	arg.sz = 0;
	arg.buffer_size = 0;
	return *this; // !! return self reference
}

// GETTERS/SETTERS:

template <typename T>
int MyVector<T>::size() const
{
	return sz;
}

template <typename T>
int MyVector<T>::get_buffer_size() const
{
	return buffer_size;
}

template <typename T>
T MyVector<T>::get(int index)
{
	index_check(index);
	return elem[index];
}

template <typename T>
void MyVector<T>::set(T x, int index)
{
	index_check(index);
	elem[index] = x;
}

// FUNZIONI:

// Funzione che rimuove l'ultimo elemento e lo returna 
template <typename T>
T MyVector<T>::pop_back()
{						   
	T temp = elem[sz - 1]; 
	--sz;				   
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
	for (int i = 0; (buffer_size - sz) < n; i++) 
		resize();								 
												 
}

// operatore<< 
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
#endif 