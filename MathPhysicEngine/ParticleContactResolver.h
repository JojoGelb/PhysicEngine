#pragma once
#include "ParticleContact.h"
#include <vector>

class ParticleContactResolver {
protected:

	//number of allowed iteration = double max contacts
	unsigned int iteration = 1;


public:
	void SetIterationNumber(unsigned int iterationNumber) { iteration = iterationNumber; }
	void ResolveContacts(std::vector<ParticleContact*>& contactArray, float frameTime);
};