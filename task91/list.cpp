#include "list.h"
#include "string.h"
#include <cstring>
#include <cstdlib>
#include <type_traits>

template<typename T>
list<T>::list() : arr(nullptr), size(0), cap(0) {}

template <typename T>
list<T>::list(const list& other) : arr(nullptr), size(0), cap(0) {
    for (int i = 0; i < other.size; ++i) {
        add(other.arr[i]);
    }
}

template <typename T>
list<T>& list<T>::operator=(const list& other) {
    if (this != &other){
        delete[] arr;
        arr = nullptr;
        size = 0;
        cap = 0;
        for (int i = 0; i < other.size; ++i)
            add(other.arr[i]);
    }
    return *this;
}

template<typename T>
list<T>::~list() {
    delete[] arr;
}

template<typename T>
void list<T>::resize() {
    cap = cap == 0 ? 10 : cap*2;
    T *new_arr = new T[cap];
    for (int i = 0; i < size; ++i)
        new_arr[i] = arr[i];
    delete[] arr;
    arr = new_arr;
}

template<typename T>
void list<T>::add(const T& item) {
    if (size >= cap) 
        resize();
    arr[size++] = item;
}

template<typename T>
int list<T>::length() const {
    return size;
}

template<typename T>
bool list<T>::contains(const char *value) const {
    T val;
    if constexpr (std::is_same<T, int>::value)
        val = atoi(value);
    else if constexpr (std::is_same<T, double>::value)
        val = atof(value);
    else
        return false;
    for (int i = 0; i < size; ++i)
        if (arr[i] == val)
            return true;
    return false;
}

template<typename T>
const T& list<T>::get(int index) const {
    return arr[index];
}

template<>
bool list<string>::contains(const char* value) const {
    for (int i = 0; i < size; ++i) {
        if (std::strcmp(arr[i].get_data(), value) == 0)
            return true;
    }
    return false;
}

template class list<int>;
template class list<double>;
template class list<string>;



