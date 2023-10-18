#pragma once
#include "Particle.h"
#include "ParticleContact.h"
#include "ParticleContactGenerator.h"

class ParticleLink : public ParticleContactGenerator {

public:
	Particle* particle[2];

	//length of the link
	float CurrentLength() const;

	unsigned int AddContact(std::vector<ParticleContact*>& contacts, unsigned int limit) const = 0;
	
	ParticleLink(Particle* particleA, Particle* particleB)
		: particle{ particleA,particleB } {};
};