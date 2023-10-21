#pragma once

#include "ParticleContactGenerator.h"

class NaiveParticleContactGenerator : public ParticleContactGenerator
{
public:
	float radius;

	//particles that could collide together
	std::vector<Particle*>* particles;




	// Hérité via ParticleContactGenerator
	unsigned int AddContact(std::vector<ParticleContact*>& contacts, unsigned int limit) const override;

	NaiveParticleContactGenerator(float radius, std::vector<Particle*>* particles)
		: radius(radius), particles(particles) {};
};