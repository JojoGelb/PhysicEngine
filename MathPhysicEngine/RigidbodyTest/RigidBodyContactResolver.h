#pragma once
#include <vector>
#include "RigidBodyContact.h"
class RigidBodyContactResolver
{
protected:

	//number of allowed iteration = double max contacts
	unsigned int iteration = 1;


public:
	void SetIterationNumber(unsigned int iterationNumber) { iteration = iterationNumber; }
	void ResolveContacts(std::vector<RigidBodyContact*>& contactArray, float frameTime);
};

