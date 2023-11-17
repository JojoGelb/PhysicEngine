#pragma once
#include "../Particle.h"
#include "ForceGenerator.h"
#include <vector>
class ParticleForceRegistry
{
private:
	struct ParticleForceEntry
	{
		Particle* particle;
		ForceGenerator* forceGenerator;
		ParticleForceEntry(Particle* particle, ForceGenerator* forceGenerator);
	};

	using Registry = std::vector<ParticleForceEntry>;
	Registry m_registry;


public:

	// Reigstry acccesor;
	void UpdateForce(float duration);
	void AddForce(Particle* particle, ForceGenerator* forceGenerator);
	void DeleteForce(Particle* particle, ForceGenerator* forceGenerator);
	void RemoveParticle(Particle* particle);

};

