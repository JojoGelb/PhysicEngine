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
<<<<<<< HEAD
	contactGenerators.push_back(new NaiveParticleContactGenerator(1.0f, &particles));
=======
	particleForceRegistry = new ParticleForceRegistry();
>>>>>>> develop
}

void MathPhysicsEngine::Update(double t,float frameTime)
{
<<<<<<< HEAD
	//Genere les forces

	//Integrate
	for (Particle* p : particles) {
=======
	UpdateSumForces(frameTime);

	for (Particle * p : particles) {
>>>>>>> develop
		p->SemiImpliciteEulerIntegration(t, (double)frameTime);
	}

	unsigned usedContacts = GenerateContacts();


	if (usedContacts) {
	
		contactResolver.ResolveContacts(particlesContact, frameTime);
	
	}

}

void MathPhysicsEngine::Shutdown()
{
	delete this->particleForceRegistry;
}

void MathPhysicsEngine::SetFinalStates(const double alpha)
{
	for (Particle* p : particles) {
		p->finalState = p->currentState * alpha + p->previousState * (1.0 - alpha);
	}
}

ParticleForceRegistry* MathPhysicsEngine::GetParticleForceRegistry()
{
	return this->particleForceRegistry;
}

void MathPhysicsEngine::UpdateSumForces(float frameTime)
{

	for (Particle* p : particles) {
		p->ClearSumForce();
	}

	particleForceRegistry->UpdateForce(frameTime);

}
MathPhysicsEngine* MathPhysicsEngine::singleton_ = nullptr;
