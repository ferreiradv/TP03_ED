#ifndef DYNAMIC_ARRAY_HPP
#define DYNAMIC_ARRAY_HPP

#include <stdexcept>// para tratamento de exceções (std::out_of_range)

template <typename T>// Classe genérica para um array dinâmico
class DynamicArray
{
private:

    T* data;

    int size;

    int capacity;

    void resize(int newCapacity);

public:

    DynamicArray();

    DynamicArray(const DynamicArray<T>& other);

    DynamicArray<T>& operator=(const DynamicArray<T>& other);

    ~DynamicArray();

    void push_back(const T& value);

    void pop_back();

    void clear();

    T& operator[](int index);

    const T& operator[](int index) const;

    int getSize() const;

    int getCapacity() const;

    bool empty() const;

    void insert(int pos, const T& value);

    void removeAt(int pos);
};

//////////////////////////////////////////////////////////
// IMPLEMENTAÇÃO
//////////////////////////////////////////////////////////

template <typename T>
DynamicArray<T>::DynamicArray()
{
    size = 0;
    capacity = 4;
    data = new T[capacity];
}

template <typename T>
DynamicArray<T>::DynamicArray(const DynamicArray<T>& other)
{
    size = other.size;
    capacity = other.capacity;

    data = new T[capacity];

    for (int i = 0; i < size; i++)
    {
        data[i] = other.data[i];
    }
}

template <typename T>
DynamicArray<T>& DynamicArray<T>::operator=(const DynamicArray<T>& other)
{
    if (this == &other)
    {
        return *this;
    }

    delete[] data;

    size = other.size;
    capacity = other.capacity;

    data = new T[capacity];

    for (int i = 0; i < size; i++)
    {
        data[i] = other.data[i];
    }

    return *this;
}

template <typename T>
DynamicArray<T>::~DynamicArray()
{
    delete[] data;
}

template <typename T>
void DynamicArray<T>::resize(int newCapacity)
{
    T* newData = new T[newCapacity];

    for (int i = 0; i < size; i++)
    {
        newData[i] = data[i];
    }

    delete[] data;

    data = newData;
    capacity = newCapacity;
}

template <typename T>
void DynamicArray<T>::push_back(const T& value)
{
    if (size == capacity)
    {
        resize(capacity * 2);
    }

    data[size] = value;
    size++;
}

template <typename T>
void DynamicArray<T>::pop_back()
{
    if (size > 0)
    {
        size--;
    }
}

template <typename T>
void DynamicArray<T>::clear()
{
    size = 0;
}

template <typename T>
T& DynamicArray<T>::operator[](int index)
{
    if (index < 0 || index >= size)
    {
        throw std::out_of_range("DynamicArray index out of range");
    }

    return data[index];
}

template <typename T>
const T& DynamicArray<T>::operator[](int index) const
{
    if (index < 0 || index >= size)
    {
        throw std::out_of_range("DynamicArray index out of range");
    }

    return data[index];
}

template <typename T>
int DynamicArray<T>::getSize() const
{
    return size;
}

template <typename T>
int DynamicArray<T>::getCapacity() const
{
    return capacity;
}

template <typename T>
bool DynamicArray<T>::empty() const
{
    return size == 0;
}

template <typename T>
void DynamicArray<T>::insert(int pos, const T& value)
{
    if (pos < 0 || pos > size)
    {
        throw std::out_of_range("DynamicArray insert position out of range");
    }

    if (size == capacity)
    {
        resize(capacity * 2);
    }

    for (int i = size; i > pos; i--)
    {
        data[i] = data[i - 1];
    }

    data[pos] = value;
    size++;
}

template <typename T>
void DynamicArray<T>::removeAt(int pos)
{
    if (pos < 0 || pos >= size)
    {
        throw std::out_of_range("DynamicArray removeAt position out of range");
    }

    for (int i = pos; i < size - 1; i++)
    {
        data[i] = data[i + 1];
    }

    size--;
}

#endif