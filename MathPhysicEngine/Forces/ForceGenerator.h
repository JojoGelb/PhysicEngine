#pragma once
#include "../Particle.h"
class ForceGenerator
{
public:
	virtual void UpdateForce(Particle* particle, float duration) = 0;
};

