#pragma once
#include "ParticleContact.h"
#include <vector>

class ParticleContactResolver {
protected:

	//number of allowed iteration
	unsigned int iteration = 10;


public:

	void ResolveContacts(std::vector<ParticleContact*>& contactArray, float frameTime);
};