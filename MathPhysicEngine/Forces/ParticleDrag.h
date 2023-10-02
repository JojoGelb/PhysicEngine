#pragma once
#include "ParticleForceGenerator.h"
class ParticleDrag :
    public ParticleForceGenerator
{
private:
    //Drag coeffiecents
    float m_k1;
    float m_k2;
public:
    //simplifed drag based on particle's velocity
    void UpdateForce(Particle* particle, float duration);
};

