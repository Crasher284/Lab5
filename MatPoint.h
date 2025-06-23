//
// Created by admin on 23.06.2025.
//

#ifndef LAB5_MATPOINT_H
#define LAB5_MATPOINT_H

#include "Vector.h"
#include "DynamicArray.h"

Vector<double> operator-(Vector<double> x, Vector<double> y){
    return x + (y * -1.0);
}

double dist(Vector<double> point, Vector<double> other){
    return (point - other).norm();
}

Vector<double> dir(Vector<double> start, Vector<double> end){
    Vector<double> vec = end - start;
    if(vec.norm()==0){
        throw std::invalid_argument("dir: zero vector don't have direction.");
    }
    return vec * (1/vec.norm());
}

class MatPoint {
public:
    MatPoint(int cnt, double m, double k, double b, double c) : cns(cnt), mass(m), k(k), b(b), c(c), coords(3), prevCoords(3), neighbors(cnt), lengths(cnt), a(3), v(3) {
        if(m<=0){
            throw std::invalid_argument("MatPoint: mass must be positive.");
        }
        if(k<0 || b<0 || c<0){
            throw std::invalid_argument("MatPoint: params cannot be negative.");
        }
        for(int i=0;i<cns;i++){
            neighbors.set(i, nullptr);
        }
    }

    MatPoint(int cnt, double m, double k, double b, double c, double x, double y, double z) : MatPoint(cnt, m, k, b, c) {
        coords.setCoord(0, x);
        coords.setCoord(1, y);
        coords.setCoord(2, z);
        prevCoords = coords;
    }


    Vector<double> getCoords() const{
        return coords;
    }

    Vector<double> getSpeed() const{
        return v;
    }

    Vector<double> getAcceleration() const{
        return a;
    }

    int getConnects() const{
        int cnt = 0;
        for(int i=0;i<cns;i++){
            if(neighbors.get(i)){
                cnt++;
            }
        }
        return cnt;
    }

    MatPoint* getNeighbor(int i) const{
        return neighbors.get(i);
    }

    void link(MatPoint* other){
        for(int i=0;i<cns;i++){
            MatPoint* current = neighbors.get(i);
            if(!current){
                neighbors.set(i, other);
                lengths.set(i, dist(this->coords, other->getCoords()));
                for(int j=0;j<other->cns;j++){
                    MatPoint* current2 = other->neighbors.get(j);
                    if(!current2){
                        other->neighbors.set(j, this);
                        other->lengths.set(j, dist(this->coords, other->getCoords()));
                        return;
                    }
                }
                throw std::out_of_range("link: maximal number of neighbors reached for other.");
            }
        }
        throw std::out_of_range("link: maximal number of neighbors reached.");
    }

    void unlink(int index){
        neighbors.set(index, nullptr);
        lengths.set(index, 0.0);
    }

    void update(Vector<double> force){
        Vector<double> r = force;
        for(int i=0;i<cns;i++){
            MatPoint* current = neighbors.get(i);
            if(!current){
                continue;
            }
            double d = dist(this->coords, current->coords);
            Vector<double> e = dir(this->coords, current->coords);
            if(d>=lengths.get(i)*b){
                unlink(i);
                continue;
            }
            // elasticy
            r += e*k*(d-lengths.get(i));
            // viscosity
            r += e*c*((current->v - v) * e);
        }
        a = r * (1/mass);
    }

    void move(double dt){
        Vector<double> prev = coords;
        Vector<double> next = coords*2 - prevCoords + a*dt*dt;
        v = (next - prevCoords)*(1/(2*dt));
        coords = next;
        prevCoords = prev;
    }

    ~MatPoint(){
        for(int i=0;i<cns;i++){
            MatPoint* current = neighbors.get(i);
            if(current){
                for(int j=0;j<current->cns;j++){
                    if(current->neighbors.get(j) == this){
                        current->unlink(j);
                        break;
                    }
                }
            }
        }
    }
private:
    int cns;
    double mass, k, b, c;
    Vector<double> coords;
    DynamicArray<MatPoint*> neighbors;
    DynamicArray<double> lengths;

    Vector<double> a, v, prevCoords;
};

#endif //LAB5_MATPOINT_H
