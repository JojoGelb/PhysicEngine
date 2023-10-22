#pragma once
#include "ParticleForceGenerator.h"
class InputForce :
    public ParticleForceGenerator
{
private:
public:
    void UpdateForce(Particle* particle, float duration);
    InputForce();
};
