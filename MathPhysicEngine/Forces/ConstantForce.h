#pragma once
#include "ParticleForceGenerator.h"
class ConstantForce :
    public ParticleForceGenerator
{
private:
    Vector3D m_constantForce;
public:
    //simplifed drag based on particle's velocity
    void UpdateForce(Particle* particle, float duration);
    ConstantForce(const Vector3D m_constantForce);
};
