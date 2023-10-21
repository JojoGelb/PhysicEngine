#pragma once
#include "ParticleForceGenerator.h"
class ParticleElasticBungee :
    public ParticleForceGenerator
{
private:
    Particle* other;
    float m_springConstant;
    float m_restLength;

public:
    void UpdateForce(Particle* particle, float duration);

    ParticleElasticBungee(Particle* other, float m_springConstant, float m_restLength);
};

