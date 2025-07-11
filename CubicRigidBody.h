//
// Created by admin on 11.07.2025.
//

#ifndef LAB5_CUBICRIGIDBODY_H
#define LAB5_CUBICRIGIDBODY_H

#include "RigidBody.h"

class CubicRigidBody : public RigidBody {

    CubicRigidBody(int nx, int ny, int nz, double mass, double k, double b, double c, double d) : RigidBody(nx, ny, nz, k, b, c) {
        if (nx <= 0 || ny <= 0 || nz <= 0) {
            std::invalid_argument("CubicRigidBody: grid dimensions must be positive.");
        }
        if (mass <= 0) {
            std::invalid_argument("CubicRigidBody: mass of body must be positive.");
        }
        if (k < 0 || b < 0 || c < 0 || d < 0) {
            std::invalid_argument("CubicRigidBody: params cannot be negative.");
        }
        num = nx * ny * nz;
        int cnt = 0;
        points = DynamicArray<MatPoint *>(num);
        double curX = 0.0, curY = 0.0, curZ = 0.0;
        for (int i = 0; i < nx; i++) {
            for (int j = 0; j < ny; j++) {
                for (int t = 0; t < nz; t++) {
                    points.set(cnt, new MatPoint(
                            3 + (i != 0 && i != nx - 1) + (j != 0 && j != ny - 1) + (t != 0 && t != nz - 1),
                            mass / num, k, b, c, curX, curY, curZ));
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
    }
};


#endif //LAB5_CUBICRIGIDBODY_H
