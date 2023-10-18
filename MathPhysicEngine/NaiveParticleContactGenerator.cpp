#include "NaiveParticleContactGenerator.h"
#include <iostream>

unsigned int NaiveParticleContactGenerator::AddContact(std::vector<ParticleContact*>& contacts, unsigned int limit) const
{
	int i, j;

	int numAddedContacts = 0;
	//avoid double contacts using this nested loop
	for (i = 0; i < particles->size(); i++) {
		Particle* p = particles->at(i);
		for (j = i+1; j < particles->size(); j++) {
			Particle* p2 = particles->at(j);
			if (p == p2) continue; //shouldn't happen now

			float distance = p->position.DistanceTo(p2->position);

			if (distance < 2 * radius) {

				ParticleContact* contact = new ParticleContact(p,p2, 1);

				contact->contactNormal = (contact->particle[0]->position - contact->particle[1]->position).Normalize();

				float maxDist = radius * 2;

				contact->penetration = maxDist - contact->particle[0]->position.DistanceTo(contact->particle[1]->position);
				std::cout << "penetration : " << contact->penetration << std::endl;
				contacts.push_back(contact);
				numAddedContacts++;
				if (contacts.size() >= limit) return numAddedContacts;

			}
		}
		return numAddedContacts;
	}

	
}
