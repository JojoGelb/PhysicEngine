#include "Sphere.h"

Sphere::Sphere(double _radius):
radius(_radius)
{
    shape = CollisionShape::SPHERE;
}
