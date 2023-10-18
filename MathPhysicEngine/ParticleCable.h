#pragma once

#include "ParticleLink.h"

class ParticleCable : public ParticleLink {
	
public:

	//create contact when distance between 2 particles > mexLength
	float maxLength;

	// bounciness of the cable
	float restitution;

	unsigned int AddContact(std::vector<ParticleContact*>& contacts, unsigned int limit) const override;

	ParticleCable(Particle* particleA, Particle* particleB, float length, float restitution)
		:ParticleLink(particleA, particleB), maxLength(length), restitution(restitution){};
};