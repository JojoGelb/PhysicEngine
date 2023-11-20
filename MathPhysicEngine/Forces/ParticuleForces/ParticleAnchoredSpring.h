#pragma once
#include "ParticuleForceGenerator.h"
class ParticleAnchoredSpring :
    public ParticuleForceGenerator
{
private:
    Vector3D anchor;
    float m_springConstant;
    float m_restLength;

public:
    void UpdateForce(Particle* particle, float duration);

    ParticleAnchoredSpring(Vector3D anchor, float m_springConstant, float m_restLength);
};

