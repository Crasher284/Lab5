//
// Created by admin on 11.07.2025.
//

#ifndef LAB5_FACECENTERRIGIDBODY_H
#define LAB5_FACECENTERRIGIDBODY_H

#include "RigidBody.h"

class FaceCenterRigidBody : public RigidBody {
    
    FaceCenterRigidBody(int nx, int ny, int nz, double mass, double k, double b, double c, double d) : RigidBody(nx, ny, nz, k, b, c) {
        if(nx<=0 || ny<=0 || nz<=0){
            std::invalid_argument("RigidBody: grid dimensions must be positive.");
        }
        if(mass<=0){
            std::invalid_argument("RigidBody: mass of body must be positive.");
        }
        if(k<0 || b<0 || c<0 || d<0){
            std::invalid_argument("RigidBody: params cannot be negative.");
        }
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
};
#endif //LAB5_FACECENTERRIGIDBODY_H
