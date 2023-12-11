#include "CollisionPrimitive.h"

Vector3D CollisionPrimitive::GetPosition() const
{
    return transform->GetPosition();
}

void CollisionPrimitive::UpdateTransformMatrix()
{
    transform = &this->rigidBody->transformMatrix; //* offset
}
