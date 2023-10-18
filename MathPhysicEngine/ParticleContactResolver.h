#pragma once
#include "ParticleContact.h"
#include <vector>

class ParticleContactResolver {
protected:

	//number of allowed iteration = double max contacts
	unsigned int iteration = 2;


public:

	void ResolveContacts(std::vector<ParticleContact*>& contactArray, float frameTime);
};