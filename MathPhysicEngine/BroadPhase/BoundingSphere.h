#pragma once

#include "Bounding.h"

class BoundingSphere: public Bounding {
public:
    BoundingSphere() = default;
    BoundingSphere(float _size) : size(_size) {}

    double GetXSize() override {
        return size;
    }

    double GetYSize() override {
        return size;
    }

    double GetZSize() override {
        return size;
    }
    
private:
    float size = 1.41f;
};