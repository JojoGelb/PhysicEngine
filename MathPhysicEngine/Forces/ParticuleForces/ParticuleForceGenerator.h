#pragma once
#include "../../Particle.h"

class ParticuleForceGenerator
{
public:
	virtual void UpdateForce(Particle* particle, float duration) = 0;
};

