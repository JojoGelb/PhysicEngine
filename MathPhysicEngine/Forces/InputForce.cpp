#include "InputForce.h"
#include <iostream>

void InputForce::UpdateForce(Particle* particle, float duration)
{
	particle->AddForce(particle->inputValues);

	std::cout << particle->inputValues << "\n";

}

InputForce::InputForce()
{
}
