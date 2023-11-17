#pragma once
#include "../ForceGenerator.h"
class InputForce :
    public ForceGenerator
{
private:
public:
    void UpdateForce(Particle* particle, float duration);
    InputForce();
};
