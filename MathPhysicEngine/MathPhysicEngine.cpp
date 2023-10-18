#include "MathPhysicEngine.h"
#include <iostream>
#include "NaiveParticleContactGenerator.h"

MathPhysicsEngine::MathPhysicsEngine()
{
}

MathPhysicsEngine::~MathPhysicsEngine()
{
	for (ParticleContact* p : particlesContact) {
		delete p;
	}
}

//Static methods should be defined outside the class.
MathPhysicsEngine* MathPhysicsEngine::GetInstance()
{
	/**
	 * This is a safer way to create an instance. instance = new Singleton is
	 * dangeruous in case two instance threads wants to access at the same time
	 */
	if (singleton_ == nullptr) {
		singleton_ = new MathPhysicsEngine();
		singleton_->Init();
	}
	return singleton_;
}

unsigned MathPhysicsEngine::GenerateContacts()
{
	//TODO OPTIMIZE
	for (ParticleContact* p : particlesContact) delete p;
	particlesContact.clear();

	unsigned limit = 10; //maxContacts at a time
	
	for (ParticleContactGenerator* g : contactGenerators) {
		unsigned used = g->AddContact(particlesContact, limit);

		if (particlesContact.size() >= limit) break;
	}

	return particlesContact.size();
}

void MathPhysicsEngine::Init()
{
	contactGenerators.push_back(new NaiveParticleContactGenerator(1.0f, &particles));
}

void MathPhysicsEngine::Update(double t,float frameTime)
{
	//Genere les forces

	//Integrate
	for (Particle* p : particles) {
		p->SemiImpliciteEulerIntegration(t, (double)frameTime);
	}

	unsigned usedContacts = GenerateContacts();


	if (usedContacts) {
	
		contactResolver.ResolveContacts(particlesContact, frameTime);
	
	}

}

MathPhysicsEngine* MathPhysicsEngine::singleton_ = nullptr;
