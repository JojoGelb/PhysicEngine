#pragma once

#include "Bounding.h"

class BoundingBox: public Bounding {
public:
    BoundingBox() = default;
    BoundingBox(float _halfSizeX, float _halfSizeY, float _halfSizeZ) : halfSizeX(_halfSizeX), halfSizeY(_halfSizeY), halfSizeZ(_halfSizeZ) {}

    double GetXSize() override {
        return halfSizeX;
    }

    double GetYSize() override {
        return halfSizeX;
    }

    double GetZSize() override {
        return halfSizeZ;
    }
    
private:
    float halfSizeX = 1;
    float halfSizeY = 1;
    float halfSizeZ = 1;
};