#include "CollisionPrimitive.h"

Vector3D CollisionPrimitive::GetPosition() const
{
    return transform->GetPosition();
}

void CollisionPrimitive::UpdateTransformMatrix()
{
    transform = &this->rigidBody->transformMatrix; //* offset
}

Vector3D CollisionPrimitive::getAxis(unsigned index) const
{
    return transform->getAxisVector(index);
}
