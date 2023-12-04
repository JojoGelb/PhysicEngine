#include "Box.h"
double Box::TransformToAxis( const Vector3D &axis) const
{
    return
    this->halfSize.x * abs(axis.DotProduct(GetPosition().x)) +
    this->halfSize.y *  abs(axis.DotProduct(GetPosition().y)) +
    this->halfSize.z *  abs(axis.DotProduct(GetPosition().z));
}

