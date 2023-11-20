#include "RigidBodyLink.h"

float RigidBodyLink::CurrentLength() const
{
    Vector3D relativePos = rigidBody[0]->position - rigidBody[1]->position;
    return relativePos.GetNorm();
}