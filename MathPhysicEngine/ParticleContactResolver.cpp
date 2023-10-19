#include "ParticleContactResolver.h"
#include <limits>
#include <iostream>
void ParticleContactResolver::ResolveContacts(std::vector<ParticleContact*>& contactArray, float frameTime)
{
	unsigned int iterationUsed = 0;
	while (iterationUsed < iteration) {
		//std::cout << "iteration : " << iterationUsed << std::endl;

		float min = std::numeric_limits<float>::max();
		unsigned maxIndex = static_cast<unsigned int>(contactArray.size());


		//Get smallest = most important collision
		for (int i = 0; i < contactArray.size(); i++) {
			float sepVel = contactArray.at(i)->CalculateSeparatingVelocity();
			//std::cout << "sepVELOCITY : " << sepVel << " max: " << max << " interpene: " << contactArray.at(i)->penetration << std::endl;
			if (sepVel < min && (sepVel < 0 || contactArray.at(i)->penetration > 0)) {
				min = sepVel;
				maxIndex = i;
			}
		}

		//std::cout << "maxIndex : " << maxIndex << " - Size: " << contactArray.size() << "max: " << max << std::endl;

		//nothing left to be done: smallest separating velocity > 0
		if (maxIndex == contactArray.size()) break;

		std::cout << "maxIndex : " << maxIndex << " - Size: " << contactArray.size() << "min: " << min << std::endl;
		contactArray.at(maxIndex)->Resolve(frameTime);

		iterationUsed++;
		
		//std::cout << "=====================\n " << std::endl;
	}
}
