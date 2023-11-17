#pragma once
#include "ParticuleForceGenerator.h"
class ParticleSpring :
    public ParticuleForceGenerator
{
private:
    Particle *other;
    float m_springConstant;
    float m_restLength;

public:
    void UpdateForce(Particle* particle, float duration);

    ParticleSpring(Particle* other, float m_springConstant, float m_restLength);
};

