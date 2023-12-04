#pragma once
#include "CollisionPrimitive.h"
#include "../../Vecteur3D.h"

class Plane : public CollisionPrimitive
{
public:
    Vector3D normal;
    double offset;
};