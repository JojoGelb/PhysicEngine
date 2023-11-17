#pragma once
#include "ParticuleForceGenerator.h"
class ParticleGravity :
    public ParticuleForceGenerator
{
private:
    Vector3D m_gravity;
public:
    void UpdateForce(Particle* particle, float duration);
    
    ParticleGravity(const Vector3D m_gravity);
};

