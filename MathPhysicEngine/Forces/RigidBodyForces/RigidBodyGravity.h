#pragma once
#include "RigidBodyForceGenerator.h"
#include "../../Rigidbody/RigidBody.h"

class RigidBodyGravity :
    public RigidBodyForceGenerator
{
private:
    Vector3D m_gravity;
public:
    void UpdateForce(RigidBody* rigidBody, float duration);
    
    RigidBodyGravity(const Vector3D m_gravity);
};

