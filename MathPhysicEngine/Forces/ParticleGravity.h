#pragma once
#include "ParticleForceGenerator.h"
class ParticleGravity :
    public ParticleForceGenerator
{
private:
    Vector3D m_gravity;
public:
    void UpdateForce(Particle* particle, float duration);
    
    ParticleGravity(const Vector3D m_gravity);
};

