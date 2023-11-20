#pragma once
#include "../Particle.h"
#include <vector>

#include "ParticuleForces/ParticuleForceGenerator.h"

class ParticleForceRegistry
{
private:
	struct ParticleForceEntry
	{
		Particle* particle;
		ParticuleForceGenerator* forceGenerator;
		ParticleForceEntry(Particle* particle, ParticuleForceGenerator* forceGenerator);
	};

	using Registry = std::vector<ParticleForceEntry>;
	Registry m_registry;


public:

	// Reigstry acccesor;
	void UpdateForce(float duration);
	void AddForce(Particle* particle, ParticuleForceGenerator* forceGenerator);
	void DeleteForce(Particle* particle, ParticuleForceGenerator* forceGenerator);
	void RemoveParticle(Particle* particle);

};

