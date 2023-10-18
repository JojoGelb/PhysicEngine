#include "ParticleContactResolver.h"
#include <limits>
#include <iostream>
void ParticleContactResolver::ResolveContacts(std::vector<ParticleContact*>& contactArray, float frameTime)
{
	unsigned int iterationUsed = 0;
	while (iterationUsed < iteration) {
		std::cout << "iteration : " << iterationUsed << std::endl;

		float max = std::numeric_limits<float>::max();
		unsigned maxIndex = static_cast<unsigned int>(contactArray.size());


		for (int i = 0; i < contactArray.size(); i++) {
			float sepVel = contactArray.at(i)->CalculateSeparatingVelocity();
			//std::cout << "sepVELOCITY : " << sepVel << " max: " << max << " interpene: " << contactArray.at(i)->penetration << std::endl;
			if (sepVel < max && (sepVel < 0 /*|| contactArray.at(i)->penetration > 0*/)) {
				max = sepVel;
				maxIndex = i;
			}
		}

		std::cout << "maxIndex : " << maxIndex << " - Size: " << contactArray.size() << "max: " << max << std::endl;

		if (maxIndex == contactArray.size()) break;

		contactArray.at(maxIndex)->Resolve(frameTime);

		iterationUsed++;
		
		//std::cout << "=====================\n " << std::endl;
	}
}
