#pragma once
#include "../ForceGenerator.h"
class ParticleDrag :
    public ForceGenerator
{
private:
    //Drag coeffiecents
    float m_k1;
    float m_k2;
public:
    //simplifed drag based on particle's velocity
    void UpdateForce(Particle* particle, float duration);
};

