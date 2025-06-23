//
// Created by admin on 09.04.2025.
//

#ifndef LAB2_DYNAMICARRAY_H
#define LAB2_DYNAMICARRAY_H
#include <stdexcept>
#include <iostream>

template <typename T>
class DynamicArray{
public:
    explicit DynamicArray(int size){
        this->size = size;
        data = new T[size];
    }

    DynamicArray() : DynamicArray(0) {}

    DynamicArray(T* items, int count) : DynamicArray(count) {
        for(int i=0;i<size;i++){
            data[i] = items[i];
        }
    }

    DynamicArray(const DynamicArray<T> & array) : DynamicArray(array.size){
        for(int i=0;i<size;i++){
            data[i] = array.data[i];
        }
    }

    T get(int index) const{
        if(index<0 || index>=size){
            throw std::out_of_range("get: Used index cannot be interpreted as index of DynamicArray.");
        }
        return data[index];
    }

    int getSize() const{
        return size;
    }

    void set(int index, T value){
        if(index<0 || index>=size){
            throw std::out_of_range("set: Used index cannot be interpreted as index of DynamicArray.");
        }
        data[index] = value;
    }

    void resize(int newSize){
        if(newSize<0){
            throw std::out_of_range("resize: Used size cannot be assigned to DynamicArray.");
        }
        T* temp = new T[newSize];
        if(newSize > size){
            memcpy(temp, data, size * sizeof *data);
        }else{
            memcpy(temp, data, newSize * sizeof *data);
        }
        delete data;
        data = temp;
        size = newSize;
    }

    T& operator[](int index){
        if(index<0 || index>=size){
            throw std::out_of_range("operator[]: Used index cannot be interpreted as index of DynamicArray.");
        }
        return data[index];
    }

    const T& operator[](int index) const{
        if(index<0 || index>=size){
            throw std::out_of_range("operator[]: Used index cannot be interpreted as index of DynamicArray.");
        }
        return data[index];
    }

    DynamicArray<T>& operator=(const DynamicArray<T>& other){
        if (this != &other) {
            delete[] data;
            size = other.size;
            data = new T[size];
            for (int i = 0; i < size; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    friend std::ostream& operator << (std::ostream &os, const DynamicArray<T> &list){
        os << "[";
        for(int i=0;i < list.size;i++){
            os << list.data[i];
            if(i<list.size-1){
                os << ", ";
            }else{
                os << "]";
            }
        }
        return os;
    }

    ~DynamicArray(){
        delete[] data;
    }

private:
    int size;
    T* data;
};

template <typename T>
std::ostream& operator << (std::ostream &os, const DynamicArray<T> &list){
    os << "[";
    for(int i=0;i < list.size;i++){
        os << list.data[i];
        if(i<list.size-1){
            os << ", ";
        }else{
            os << "]";
        }
    }
    return os;
}

#endif //LAB2_DYNAMICARRAY_H
