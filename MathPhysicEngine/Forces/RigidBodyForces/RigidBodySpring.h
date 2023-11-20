#pragma once
#include "RigidBodyForceGenerator.h"
class RigidBodySpring :
    public RigidBodyForceGenerator
{
private:
    RigidBody *other;
    float m_springConstant;
    float m_restLength;

    Vector3D connectionPoint;
    Vector3D otherConnectionPoint;

public:
    void UpdateForce(RigidBody* rigidBody, float duration);

    RigidBodySpring(RigidBody* other, Vector3D connectionPoint,Vector3D otherConnectionPoint, float m_springConstant, float m_restLength);
};

