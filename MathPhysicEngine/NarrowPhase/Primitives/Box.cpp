#include "Box.h"

Box::Box(const Vector3D& _halfSize):
halfSize(_halfSize)
{
    shape = BOX;
}

double Box::TransformToAxis( const Vector3D &axis) const
{
    return
    this->halfSize.x * abs(axis.DotProduct(GetPosition().x)) +
    this->halfSize.y *  abs(axis.DotProduct(GetPosition().y)) +
    this->halfSize.z *  abs(axis.DotProduct(GetPosition().z));
}

