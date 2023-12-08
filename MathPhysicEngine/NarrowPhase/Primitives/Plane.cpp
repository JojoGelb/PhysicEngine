#include "Plane.h"

Plane::Plane(const Vector3D& _normal):
normal(_normal)
{
    shape = PLANE;
}
