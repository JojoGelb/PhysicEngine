#pragma once
#include "ParticuleForceGenerator.h"
class ParticleElasticBungee :
    public ParticuleForceGenerator
{
private:
    Particle* other;
    float m_springConstant;
    float m_restLength;

public:
    void UpdateForce(Particle* particle, float duration);

    ParticleElasticBungee(Particle* other, float m_springConstant, float m_restLength);
};

