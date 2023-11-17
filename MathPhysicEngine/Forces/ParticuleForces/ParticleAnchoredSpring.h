#pragma once
#include "../ForceGenerator.h"
class ParticleAnchoredSpring :
    public ForceGenerator
{
private:
    Vector3D anchor;
    float m_springConstant;
    float m_restLength;

public:
    void UpdateForce(Particle* particle, float duration);

    ParticleAnchoredSpring(Vector3D anchor, float m_springConstant, float m_restLength);
};

