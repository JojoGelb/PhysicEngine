#pragma once
#include <vector>
#include "RigidBodyContactTest.h"
class RigidBodyContactResolver
{
protected:

	//number of allowed iteration = double max contacts
	unsigned int iteration = 1;


public:
	void SetIterationNumber(unsigned int iterationNumber) { iteration = iterationNumber; }
	void ResolveContacts(std::vector<RigidBodyContactTest*>& contactArray, float frameTime);
};

