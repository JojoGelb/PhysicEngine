#pragma once
#include "../ForceGenerator.h"
class ParticleGravity :
    public ForceGenerator
{
private:
    Vector3D m_gravity;
public:
    void UpdateForce(Particle* particle, float duration);
    
    ParticleGravity(const Vector3D m_gravity);
};

