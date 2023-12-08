#pragma once
#include "CollisionPrimitive.h"
#include "../../Vecteur3D.h"

class Plane : public CollisionPrimitive
{
public:
    Plane(const Vector3D& _normal);
    
    Vector3D normal;
};