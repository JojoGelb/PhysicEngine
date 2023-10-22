#pragma once
#include "ParticleForceGenerator.h"
class ConstantForce :
    public ParticleForceGenerator
{
private:
    Vector3D m_constantForce;
public:
    void UpdateForce(Particle* particle, float duration);
    ConstantForce(const Vector3D m_constantForce);
};
