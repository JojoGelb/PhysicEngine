#include "Plane.h"

Plane::Plane(const Vector3D& _direction):
direction(_direction)
{
    shape = PLANE;
}
