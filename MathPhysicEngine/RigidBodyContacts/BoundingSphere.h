#pragma once
#include "../Vecteur3D.h"

class BoundingSphere
{
public:

    Vector3D center;
    double radius;

    BoundingSphere(const Vector3D &center, float radius) : center(center), radius(radius) {}
    
    //Encapsulate the two others
    BoundingSphere(const BoundingSphere &one, const BoundingSphere &two);

    bool overlaps(const BoundingSphere *other) const;
    float GetSize() const;

    double GetGrowth(const BoundingSphere &other) const; 
};