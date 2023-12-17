#include "Plane.h"

Plane::Plane(const Vector3D& _direction, const double& _offset):
    direction(_direction),offset(_offset)
{
    shape = CollisionShape::PLANE;
}
