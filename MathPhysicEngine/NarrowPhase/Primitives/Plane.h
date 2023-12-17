#pragma once
#include "CollisionPrimitive.h"
#include "../../Vecteur3D.h"

class Plane : public CollisionPrimitive
{
public:
    Plane(const Vector3D& _normal,const double& _offset);
    
    Vector3D direction;
    double offset;
};