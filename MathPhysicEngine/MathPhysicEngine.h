#pragma once
#include "Vecteur3D.h"
#include "Particle.h"
#include <vector>
#include "Forces/ParticleForceRegistry.h"
#include "ParticleContact.h"
#include "ParticleContactResolver.h"
#include "ParticleContactGenerator.h"
class MathPhysicsEngine {

protected:
	MathPhysicsEngine();
	static MathPhysicsEngine* singleton_;

public:

	//Can't be cloned
	MathPhysicsEngine(MathPhysicsEngine& other) = delete;
	//Can't be assignated
	void operator=(const MathPhysicsEngine&) = delete;

	~MathPhysicsEngine();

	static MathPhysicsEngine* GetInstance();


	void Update(double t, float frameTime);
	void Shutdown();

	void AddParticle(Particle* p) { particles.push_back(p); }
	void RemoveParticle(Particle* p) { particles.erase(std::remove(particles.begin(), particles.end(), p), particles.end()); }
	void SetFinalStates(const double alpha);

	
	unsigned GenerateContacts();


	ParticleForceRegistry* GetParticleForceRegistry();
private:
	void UpdateSumForces(float frameTime);
	ParticleForceRegistry* particleForceRegistry;

	void Init();
	
	std::vector<Particle*> particles;

	ParticleContactResolver contactResolver;

	std::vector<ParticleContactGenerator*> contactGenerators;

	std::vector<ParticleContact*> particlesContact;



};
