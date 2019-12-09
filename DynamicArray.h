//Programmer's name: Jiho Kim
//Programmer's ID:1735396
#ifndef DYNAMICARRAY_H
#define DYNAMICARRAY_H
template<typename V>
class DynamicArray // class Array
{
private: // member variables
    V *values;
    int cap;
    V dummy;
public: // member function
    DynamicArray(int = 2);
    DynamicArray(const DynamicArray<V>&);
    ~DynamicArray() {delete [] values;}
    DynamicArray<V>& operator=(const DynamicArray<V>&);
    int capacity() const {return cap;}
    void capacity(int);
    const V& operator[](int) const;
    V& operator[](int);
};

template<typename V>
DynamicArray<V>::DynamicArray(int cap) // default constructor
{
    this->cap = cap;
    values = new V[cap];
    for(int i = 0; i < cap; i++)
    {
        values[i] = V(); // initialize default value
    }
    dummy = V(); // initialize dummy
}

template<typename V>
DynamicArray<V>::DynamicArray(const DynamicArray<V>& original) // copy constructor
{
    cap = original.cap;
    values = new V[cap];
    for(int i = 0; i < cap; i++)
    {
        values[i] = original.values[i];
    }
    dummy = original.dummy;
}

template<typename V>
DynamicArray<V>& DynamicArray<V>::operator=(const DynamicArray<V>& original) // assignment operator
{
    if(this != &original)
    {
        delete [] values;
        cap = original.cap;
        values = new V[cap];
        for(int i = 0; i < cap; i++)
        {
            values[i] = original.values[i];
        }
        dummy = original.dummy;
    }
    return *this;
}

template<typename V>
void DynamicArray<V>::capacity(int cap)
{
    V* temp = new V[cap];
    int limit = ( cap < this->cap ? cap: this->cap);
    for(int i = 0; i < limit; i++)
    {
        temp[i] = values[i];
    }
    for(int i = limit; i < cap; i++)
    {
        temp[i] = V();
    }
    delete [] values;
    values = temp;
    this->cap = cap;
}

template<typename V>
const V& DynamicArray<V>::operator[](int index) const // []operator getter
{
    if( index < 0 || index >= cap)
    {
      return dummy;// if index is out of range, return default value
    }
    return values[index]; // return value
}

template<typename V>
V& DynamicArray<V>::operator[](int index) // []operator setter
{
    if( index < 0)
    {
        return dummy; // if index is out of range, return default value
    }
    if( index >= cap)
    {
        capacity(2 * index);
    }
    return values[index]; // return value
}
#endif // DYNAMICARRAY_H


