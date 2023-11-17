#pragma once
#include "../ForceGenerator.h"
class ParticleSpring :
    public ForceGenerator
{
private:
    Particle *other;
    float m_springConstant;
    float m_restLength;

public:
    void UpdateForce(Particle* particle, float duration);

    ParticleSpring(Particle* other, float m_springConstant, float m_restLength);
};

