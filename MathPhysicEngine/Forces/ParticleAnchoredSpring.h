#pragma once
#include "ParticleForceGenerator.h"
class ParticleAnchoredSpring :
    public ParticleForceGenerator
{
private:
    Vector3D anchor;
    float m_springConstant;
    float m_restLength;

public:
    void UpdateForce(Particle* particle, float duration);

    ParticleAnchoredSpring(Vector3D anchor, float m_springConstant, float m_restLength);
};

