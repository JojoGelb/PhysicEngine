#pragma once
#include "CollisionPrimitive.h"
#include "../../Vecteur3D.h"

class Box : public CollisionPrimitive
{
public:
    Box(const Vector3D& _halfSize);
    
    Vector3D halfSize;
    
    double TransformToAxis(const Vector3D& axis) const;
};
