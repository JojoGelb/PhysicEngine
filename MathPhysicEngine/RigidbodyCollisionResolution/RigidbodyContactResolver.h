#pragma once
#include "../RigidbodyTest/RigidBodyContact.h"
#include <vector>

class RigidbodyContactResolver {
protected:

	//number of allowed iteration = double max contacts
	unsigned int iteration = 1;


public:
	void SetIterationNumber(unsigned int iterationNumber) { iteration = iterationNumber; }
	//void ResolveContacts(std::vector<RigidBodyContact*>& contactArray, float frameTime);
};