//
// Created by admin on 23.06.2025.
//

#ifndef LAB5_RIGIDBODY_H
#define LAB5_RIGIDBODY_H

#include "MatPoint.h"
#include <iostream>
#include <random>

enum Lattice {CUBE, CUBEFACES, CUBEVOLUME};

class RigidBody {
public:
    RigidBody(int nx, int ny, int nz, double mass, double k, double b, double c, double d, Lattice type) : nx(nx), ny(ny), nz(nz), k(k), b(b), c(c) {
        if(nx<=0 || ny<=0 || nz<=0){
            std::invalid_argument("RigidBody: grid dimensions must be positive.");
        }
        if(mass<=0){
            std::invalid_argument("RigidBody: mass of body must be positive.");
        }
        if(k<0 || b<0 || c<0 || d<0){
            std::invalid_argument("RigidBody: params cannot be negative.");
        }
        if(type == CUBE){
            num = nx*ny*nz;
            int cnt=0;
            points = DynamicArray<MatPoint*>(num);
            double curX=0.0, curY=0.0, curZ=0.0;
            for(int i=0;i<nx;i++) {
                for (int j = 0; j < ny; j++) {
                    for (int t = 0; t < nz; t++) {
                        points.set(cnt, new MatPoint(3+(i!=0 && i!=nx-1)+(j!=0 && j!=ny-1)+(t!=0 && t!=nz-1), mass/num, k, b, c, curX, curY, curZ));
                        if(t!=0){
                            points.get(cnt)->link(points.get(cnt-1));
                        }
                        if(j!=0){
                            points.get(cnt)->link(points.get(cnt-nz));
                        }
                        if(i!=0){
                            points.get(cnt)->link(points.get(cnt-ny*nz));
                        }
                        curZ+=d;
                        cnt++;
                    }
                    curY+=d;
                    curZ = 0.0;
                }
                curX+=d;
                curY = 0.0;
            }
        }
        if(type == CUBEFACES){
            num = nx*ny*nz+(nx-1)*(ny-1)*nz+(nx-1)*ny*(nz-1)+nx*(ny-1)*(nz-1);
            int cnt=0;
            points = DynamicArray<MatPoint*>(num);
            double curX=0.0, curY=0.0, curZ=0.0;
            for(int i=0;i<nx;i++) {
                for (int j = 0; j < ny; j++) {
                    for (int t = 0; t < nz; t++) {
                        int place = (i!=0 && i!=nx-1)+(j!=0 && j!=ny-1)+(t!=0 && t!=nz-1);
                        points.set(cnt, new MatPoint(6+place*2+(place*(place+1))/2, mass/num, k, b, c, curX, curY, curZ));
                        if(t!=0){
                            points.get(cnt)->link(points.get(cnt-1));
                        }
                        if(j!=0){
                            points.get(cnt)->link(points.get(cnt-nz));
                        }
                        if(i!=0){
                            points.get(cnt)->link(points.get(cnt-ny*nz));
                        }
                        curZ+=d;
                        cnt++;
                    }
                    curY+=d;
                    curZ = 0.0;
                }
                curX+=d;
                curY = 0.0;
            }
            std::cout<<"\n";
            curX=0.0, curY=d/2, curZ=d/2;
            for(int i=0;i<nx;i++){
                for(int j=0;j<ny-1;j++){
                    for(int t=0;t<nz-1;t++){
                        points.set(cnt, new MatPoint(4, mass/num, k, b, c, curX, curY, curZ));
                        points.get(cnt)->link(points.get(i*ny*nz+j*nz+t));
                        points.get(cnt)->link(points.get(i*ny*nz+(j+1)*nz+t));
                        points.get(cnt)->link(points.get(i*ny*nz+j*nz+t+1));
                        points.get(cnt)->link(points.get(i*ny*nz+(j+1)*nz+t+1));
                        curZ+=d;
                        cnt++;
                    }
                    curY+=d;
                    curZ = d/2;
                }
                curX+=d;
                curY = d/2;
            }
            std::cout<<"\n";
            curX=d/2, curY=0.0, curZ=d/2;
            for(int j=0;j<ny;j++){
                for(int i=0;i<nx-1;i++){
                    for(int t=0;t<nz-1;t++){
                        points.set(cnt, new MatPoint(4, mass/num, k, b, c, curX, curY, curZ));
                        points.get(cnt)->link(points.get(i*ny*nz+j*nz+t));
                        points.get(cnt)->link(points.get((i+1)*ny*nz+j*nz+t));
                        points.get(cnt)->link(points.get(i*ny*nz+j*nz+t+1));
                        points.get(cnt)->link(points.get((i+1)*ny*nz+j*nz+t+1));
                        curZ+=d;
                        cnt++;
                    }
                    curX+=d;
                    curZ = d/2;
                }
                curY+=d;
                curX = d/2;
            }
            std::cout<<"\n";
            curX=d/2, curY=d/2, curZ=0.0;
            for(int t=0;t<nz;t++){
                for(int i=0;i<nx-1;i++){
                    for(int j=0;j<ny-1;j++){
                        points.set(cnt, new MatPoint(4, mass/num, k, b, c, curX, curY, curZ));
                        points.get(cnt)->link(points.get(i*ny*nz+j*nz+t));
                        points.get(cnt)->link(points.get((i+1)*ny*nz+j*nz+t));
                        points.get(cnt)->link(points.get(i*ny*nz+(j+1)*nz+t));
                        points.get(cnt)->link(points.get((i+1)*ny*nz+(j+1)*nz+t));
                        curY+=d;
                        cnt++;
                    }
                    curX+=d;
                    curY = d/2;
                }
                curZ+=d;
                curX = d/2;
            }
        }
        if(type == CUBEVOLUME) {
            num = nx * ny * nz + (nx - 1) * (ny - 1) * (nz - 1);
            int cnt = 0;
            points = DynamicArray<MatPoint *>(num);
            double curX = 0.0, curY = 0.0, curZ = 0.0;
            for (int i = 0; i < nx; i++) {
                for (int j = 0; j < ny; j++) {
                    for (int t = 0; t < nz; t++) {
                        int place = (i != 0 && i != nx - 1) + (j != 0 && j != ny - 1) + (t != 0 && t != nz - 1);
                        points.set(cnt,
                                   new MatPoint(3 + place + pow(2, place), mass / num, k, b, c, curX,
                                                curY, curZ));
                        if (t != 0) {
                            points.get(cnt)->link(points.get(cnt - 1));
                        }
                        if (j != 0) {
                            points.get(cnt)->link(points.get(cnt - nz));
                        }
                        if (i != 0) {
                            points.get(cnt)->link(points.get(cnt - ny * nz));
                        }
                        curZ += d;
                        cnt++;
                    }
                    curY += d;
                    curZ = 0.0;
                }
                curX += d;
                curY = 0.0;
            }
            curX = d/2, curY = d/2, curZ = d/2;
            for(int i=0;i<nx-1;i++){
                for(int j=0;j<ny-1;j++){
                    for(int t=0;t<nz-1;t++){
                        points.set(cnt, new MatPoint(8, mass/num, k, b, c, curX, curY, curZ));
                        points.get(cnt)->link(points.get(i*ny*nz+j*nz+t));
                        points.get(cnt)->link(points.get((i+1)*ny*nz+j*nz+t));
                        points.get(cnt)->link(points.get(i*ny*nz+(j+1)*nz+t));
                        points.get(cnt)->link(points.get((i+1)*ny*nz+(j+1)*nz+t));
                        points.get(cnt)->link(points.get(i*ny*nz+j*nz+t+1));
                        points.get(cnt)->link(points.get((i+1)*ny*nz+j*nz+t+1));
                        points.get(cnt)->link(points.get(i*ny*nz+(j+1)*nz+t+1));
                        points.get(cnt)->link(points.get((i+1)*ny*nz+(j+1)*nz+t+1));
                        curZ+=d;
                        cnt++;
                    }
                    curY += d;
                    curZ = d/2;
                }
                curX += d;
                curY = d/2;
            }
        }
    }

    int getNum() const {
        return num;
    }

    Vector<double> getPointCoords(int index) {
        if (index < 0 || index >= num) {
            throw std::out_of_range("getPointCoords: invalid point index");
        }
        return points.get(index)->getCoords();
    }

    int getPointConnects(int index) const {
        if (index < 0 || index >= num) {
            throw std::out_of_range("Invalid point index");
        }
        return points.get(index)->getConnects();
    }

    int getPointNeighborIndex(int pointIndex, int neighborIndex) const {
        if (pointIndex < 0 || pointIndex >= num) {
            throw std::out_of_range("Invalid point index");
        }
        MatPoint* neighbor = points.get(pointIndex)->getNeighbor(neighborIndex);
        if(!neighbor){
            return -1;
        }
        for (int i = 0; i < num; i++) {
            if (points.get(i) == neighbor) {
                return i;
            }
        }
        return -1;
    }

    void update(int defType, double power){
        Vector<double> brown(3), force(3), block(3);
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::normal_distribution<double> dist(0.0, 0.001);
        if(defType == 0){
            block.setCoord(0, 1.0);
            block.setCoord(1, 1.0);
            block.setCoord(2, 1.0);
            for (int i = 0; i < num; i++) {
                brown.setCoord(0, dist(gen));
                brown.setCoord(1, dist(gen));
                brown.setCoord(2, dist(gen));
                points.get(i)->update(brown, block);
            }
        }else if(defType == 1){
            // compression
            force.setCoord(0, power);
            block.setCoord(0, 1.0);
            block.setCoord(1, 1.0);
            block.setCoord(2, 1.0);
            for (int i = 0; i < num; i++) {
                brown.setCoord(0, dist(gen));
                brown.setCoord(1, dist(gen));
                brown.setCoord(2, dist(gen));
                if(i<ny*nz){
                    points.get(i)->update(force+brown, block);
                }else if(i>=(nx-1)*ny*nz && i < nx*ny*nz){
                    points.get(i)->update(force*(-1)+brown, block);
                }else{
                    points.get(i)->update(brown, block);
                }
            }
        }else if(defType == 2){
            // stretching
            force.setCoord(0, power);
            block.setCoord(0, 1.0);
            block.setCoord(1, 1.0);
            block.setCoord(2, 1.0);
            for (int i = 0; i < num; i++) {
                brown.setCoord(0, dist(gen));
                brown.setCoord(1, dist(gen));
                brown.setCoord(2, dist(gen));
                if(i<ny*nz){
                    points.get(i)->update(force*(-1)+brown, block);
                }else if(i>=(nx-1)*ny*nz && i < nx*ny*nz){
                    points.get(i)->update(force+brown, block);
                }else{
                    points.get(i)->update(brown, block);
                }
            }
        }else if(defType == 3){
            // shift
            force.setCoord(1, power);
            block.setCoord(0, 1.0);
            block.setCoord(1, 1.0);
            block.setCoord(2, 1.0);
            for (int i = 0; i < num; i++) {
                brown.setCoord(0, dist(gen));
                brown.setCoord(1, dist(gen));
                brown.setCoord(2, dist(gen));
                if(i<ny*nz){
                    points.get(i)->update(force+brown, force);
                }else if(i>=(nx-1)*ny*nz && i < nx*ny*nz){
                    points.get(i)->update(force*(-1)+brown, force*(-1));
                }else{
                    points.get(i)->update(brown, block);
                }
            }
        }else if(defType == 4){
            // bend
            force.setCoord(1, power);
            block.setCoord(0, 1.0);
            block.setCoord(1, 1.0);
            block.setCoord(2, 1.0);
            for (int i = 0; i < num; i++) {
                brown.setCoord(0, dist(gen));
                brown.setCoord(1, dist(gen));
                brown.setCoord(2, dist(gen));
                if(i<ny*nz){
                    points.get(i)->update(brown, Vector<double>(3));
                }else if(i>=(nx-1)*ny*nz && i < nx*ny*nz){
                    points.get(i)->update(brown, Vector<double>(3));
                }else if(nx/2==i/(ny*nz) || (nx-1)/2==i/(ny*nz)){
                    if(nx/2==(nx-1)/2){
                        points.get(i)->update(force*2+brown, block);
                    }else{
                        points.get(i)->update(force+brown, block);
                    }
                }else{
                    points.get(i)->update(brown, block);
                }
            }
        }
    }

    void move(double dt){
        for(int i=0;i<num;i++){
            points.get(i)->move(dt);
        }
    }

    ~RigidBody(){
        for(int i=0;i<num;i++){
            delete points.get(i);
        }
    }
private:
    int nx, ny, nz, num;
    double k, b, c;
    DynamicArray<MatPoint*> points;
};

#endif //LAB5_RIGIDBODY_H
