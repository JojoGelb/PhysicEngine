#pragma once
#include "ParticuleForceGenerator.h"
class InputForce :
    public ParticuleForceGenerator
{
private:
public:
    void UpdateForce(Particle* particle, float duration);
    InputForce();
};
