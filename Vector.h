//
// Created by admin on 14.05.2025.
//

#ifndef LAB3_VECTOR_H
#define LAB3_VECTOR_H

#include <stdexcept>
#include <concepts>
#include "Linal.h"

enum NormType { L1, L2, LInf};

template <linal::Field T>
class Vector {
public:
    explicit Vector(int dim) : dim(dim){
        if(dim<0){
            throw std::invalid_argument("Vector: Vector size cannot be negative.");
        }
        data = new T[dim];
        for(int i=0;i<dim;i++){
            data[i] = T{};
        }
    }

    Vector(T* array, int dim) : dim(dim){
        if(dim<0){
            throw std::invalid_argument("Vector: Vector size cannot be negative.");
        }
        if (!array && dim > 0) {
            throw std::invalid_argument("Vector: Null array provided for non-zero dimension.");
        }
        data = new T[dim];
        for(int i=0;i<dim;i++){
            if(array){
                data[i] = array[i];
            }else{
                data[i] = T{};
            }
        }
    }

    Vector(const Vector& other) : dim(other.dim){
        data = new T[dim];
        for(int i=0;i<dim;i++){
            data[i] = other.data[i];
        }
    }

    int getDim() const{
        return dim;
    }

    T getCoord(int x) const{
        if(x<0 || x>=dim){
            throw std::out_of_range("getCoord: Invalid coordinate index.");
        }
        return data[x];
    }

    void setCoord(int x, T value){
        if(x<0 || x>=dim){
            throw std::out_of_range("setCoord: Invalid coordinate index.");
        }
        data[x] = value;
    }

    Vector<T> operator+(const Vector<T>& other) const{
        if (other.getDim() != dim) {
            throw std::invalid_argument("+: Vectors must have same size to be added to each other.");
        }
        Vector<T> out(dim);
        for (int i = 0; i < dim; i++) {
            out.setCoord(i, data[i] + other.getCoord(i));
        }
        return out;
    }

    Vector<T>& operator+=(const Vector<T>& other){
        if (other.getDim() != dim) {
            throw std::invalid_argument("+=: Vectors must have same size to be added to each other.");
        }
        for(int i=0;i<dim;i++){
            data[i] = data[i] + other.getCoord(i);
        }
        return *this;
    }

    Vector<T> operator*(T other) const{
        Vector<T> out(dim);
        for(int i=0;i<dim;i++){
            out.setCoord(i, data[i]*other);
        }
        return out;
    }

    Vector<T>& operator*=(T other){
        for(int i=0;i<dim;i++){
            data[i] = data[i] * other;
        }
        return *this;
    }

    double norm(NormType type = L2) const requires linal::EuclideanField<T> {
        if (dim == 0) return 0.0;
        double out = 0.0;
        if (type == L2){
            for(int i=0;i<dim;i++){
                double a = linal::abs(data[i]);
                out += a * a;
            }
            out = std::sqrt(out);
        }else if(type == L1){
            for(int i=0;i<dim;i++){
                out += linal::abs(data[i]);
            }
        }else{ //LInf
            out = linal::abs(data[0]);
            for(int i=1;i<dim;i++){
                double a = linal::abs(data[i]);
                if (a>out) out = a;
            }
        }
        return out;
    }

    T scalar_product(const Vector<T>& other) const requires linal::EuclideanField<T> {
        if (other.getDim() != dim) {
            throw std::invalid_argument("scalar_product: Vectors must have same size to be multiplied to each other.");
        }
        T out = T{};
        for(int i=0;i<dim;i++){
            out = out + data[i] * linal::conjugate(other.data[i]);
        }
        return out;
    }

    T operator*(const Vector<T>& other) const requires linal::EuclideanField<T>{
        return scalar_product(other);
    }

    Vector<T>& operator=(const Vector<T>& other) {
        if (this != &other) {
            delete[] data;
            dim = other.dim;
            data = new T[dim];
            for (int i = 0; i < dim; i++) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    bool operator==(const Vector<T>& other) const {
        if (dim != other.dim) return false;
        for (int i = 0; i < dim; i++) {
            if (data[i] != other.data[i]) return false;
        }
        return true;
    }

    ~Vector(){
        delete[] data;
    }
private:
    T* data;
    int dim;
};

template <typename T>
std::ostream& operator<< (std::ostream& os, const Vector<T>& v){
    os << "{";
    if(v.getDim()>0) {
        for (int i = 0; i < v.getDim() - 1; i++) {
            os << v.getCoord(i) << " ";
        }
        os << v.getCoord(v.getDim() - 1);
    }
    os << "}";
    return os;
}

template <linal::Field T>
Vector<T> operator*(const T& scalar, const Vector<T>& vec) {
    return vec * scalar;
}

#endif //LAB3_VECTOR_H
