#include "InputForce.h"
#include <iostream>

void InputForce::UpdateForce(Particle* particle, float duration)
{
	particle->AddForce(particle->inputValues);
}

InputForce::InputForce()
{
}
