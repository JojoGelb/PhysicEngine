#include "Box.h"

Box::Box(const Vector3D& _halfSize):
halfSize(_halfSize)
{
    shape = CollisionShape::BOX;
}

double Box::TransformToAxis( const Vector3D &axis) const
{
    return
    this->halfSize.x * abs(axis.DotProduct(getAxis(0))) +
    this->halfSize.y *  abs(axis.DotProduct(getAxis(1))) +
    this->halfSize.z *  abs(axis.DotProduct(getAxis(2)));
}

