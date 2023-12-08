#pragma once
#include "CollisionPrimitive.h"

class Sphere : public CollisionPrimitive
{
public:
    Sphere(double _radius);

    double radius;
};
