#pragma once

#include "ParticleLink.h"

class ParticleRod : public ParticleLink {

public:

	float length;

	// H�rit� via ParticleLink
	unsigned int AddContact(std::vector<ParticleContact*>& contacts, unsigned int limit) const override;
	ParticleRod(Particle* particleA, Particle* particleB, float length)
		:ParticleLink(particleA, particleB), length(length) {};
};