#pragma once
#include "ParticuleForceGenerator.h"
class ConstantForce :
    public ParticuleForceGenerator
{
private:
    Vector3D m_constantForce;
public:
    void UpdateForce(Particle* particle, float duration);
    ConstantForce(const Vector3D m_constantForce);
};
