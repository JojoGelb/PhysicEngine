#include "RigidBodyContactResolver.h"

#include <limits>
#include <iostream>
void RigidBodyContactResolver::ResolveContacts(std::vector<RigidBodyContact*>& contactArray, float frameTime)
{
	unsigned int iterationUsed = 0;
	while (iterationUsed < iteration) {

		float min = 0;
		unsigned maxIndex = static_cast<unsigned int>(contactArray.size());


		//Get smallest = most important collision
		for (int i = 0; i < contactArray.size(); i++) {
			float sepVel = contactArray.at(i)->CalculateSeparatingVelocity();
			/*PATCH contactArray.at(i)->penetration != 0 is an added condition that "patch" the issue where
			interpenetration was calculated several times on the same contact but the penetration value wasn't changed
			resolving in objects moving far away*/
			if (sepVel < min && contactArray.at(i)->penetration != 0) {
				min = sepVel;
				maxIndex = i;
			}
		}

		//nothing left to be done: smallest separating velocity > 0
		if (maxIndex == contactArray.size()) break;

		contactArray.at(maxIndex)->Resolve(frameTime);

		iterationUsed++;

	}
}