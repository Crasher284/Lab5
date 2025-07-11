//
// Created by admin on 23.06.2025.
//

#ifndef LAB5_MATPOINT_H
#define LAB5_MATPOINT_H

#include "Vector.h"
#include "DynamicArray.h"

Vector<double> operator-(const Vector<double>& x, const Vector<double>& y){
    return x + (y * -1.0);
}

double dist(const Vector<double>& point, const Vector<double>& other){
    return (point - other).norm();
}

Vector<double> dir(const Vector<double>& start, const Vector<double>& end){
    Vector<double> vec = end - start;
    if(vec.norm()==0){
        throw std::invalid_argument("dir: zero vector don't have direction.");
    }
    return vec * (1/vec.norm());
}

class MatPoint {
public:
    MatPoint(int cnt, double m, double d, double u, double v) : cns(cnt), mass(m), dens(d), unbreak(u), viscos(v), coords(3), prevCoords(3), neighbors(cnt), lengths(cnt), a(3), v(3), block(3) {
        if(m<=0){
            throw std::invalid_argument("MatPoint: mass must be positive.");
        }
        if(d < 0 || u < 0 || v < 0){
            throw std::invalid_argument("MatPoint: params cannot be negative.");
        }
        for(int i=0;i<cns;i++){
            neighbors.set(i, nullptr);
        }
    }

    MatPoint(int cnt, double m, double d, double u, double v, double x, double y, double z) : MatPoint(cnt, m, d, u, v) {
        coords.setCoord(0, x);
        coords.setCoord(1, y);
        coords.setCoord(2, z);
        prevCoords = coords;
    }


    [[nodiscard]] Vector<double> getCoords() const{
        return coords;
    }

    [[nodiscard]] int getConnects() const{
        int cnt = 0;
        for(int i=0;i<cns;i++){
            if(neighbors.get(i)){
                cnt++;
            }
        }
        return cnt;
    }

    [[nodiscard]] MatPoint* getNeighbor(int i) const{
        return neighbors.get(i);
    }

    void link(MatPoint* other){
        if(other == nullptr){
            throw std::invalid_argument("link: null pointer was given.");
        }else if(other == this){
            throw std::invalid_argument("link: MatPoint cannot be linked to itself.");
        }
        for(int i=0;i<cns;i++){
            MatPoint* current = neighbors.get(i);
            if(current == other){
                throw std::invalid_argument("link: points already linked.");
            }
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

    double getDeformation(int index){
        if(index < 0 || index >= cns){
            throw std::out_of_range("getDeformation: point doesn't have such index.");
        }
        double dst = dist(this->coords, neighbors.get(index)->coords), l = lengths.get(index);
        if(dst >= l){
            return (dst - l) / (l * (unbreak - 1));
        }else{
            return (dst - l) / l;
        }
    }

    void update(const Vector<double>& force, const Vector<double>& blck){
        block = blck;
        Vector<double> r = force;
        for(int i=0;i<cns;i++){
            MatPoint* current = neighbors.get(i);
            if(!current){
                continue;
            }
            double dst = dist(this->coords, current->coords);
            Vector<double> e = dir(this->coords, current->coords);
            if(dst >= lengths.get(i) * unbreak){
                unlink(i);
                continue;
            }
            // density
            r += e * dens * (dst - lengths.get(i));
            // viscosity
            r += e * viscos * ((current->v - v) * e);
        }
        a = r * (1/mass);
        for(int i=0;i<3;i++){
            if(block.getCoord(i)==0){
                a.setCoord(i, 0);
            }
        }
    }

    void move(double dt){
        if(dt<=0){
            throw std::invalid_argument("move: dt must be positive.");
        }
        Vector<double> prev = coords;
        Vector<double> next = Vector<double>(3);
        next = coords*2 - prevCoords + a*dt*dt;
        for(int i=0;i<3;i++){
            if(block.getCoord(i)==0){
                next.setCoord(i, coords.getCoord(i));
            }
        }
        v = (next - prevCoords)*(1/(2*dt));
        for(int i=0;i<3;i++){
            if(block.getCoord(i)==0){
                v.setCoord(i, 0);
            }
        }
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
    double mass, dens, unbreak, viscos;
    Vector<double> coords;
    DynamicArray<MatPoint*> neighbors;
    DynamicArray<double> lengths;

    Vector<double> a, v, prevCoords, block;
};

#endif //LAB5_MATPOINT_H
