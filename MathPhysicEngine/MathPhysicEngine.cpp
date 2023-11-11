#include "MathPhysicEngine.h"
#include <iostream>
#include "Collisions/NaiveParticleContactGenerator.h"

MathPhysicsEngine::MathPhysicsEngine()
{
}

MathPhysicsEngine::~MathPhysicsEngine()
{
	for (ParticleContact* p : particlesContact) {
		delete p;
	}

	delete particleForceRegistry;

	for (ParticleContactGenerator* p : contactGenerators) {
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

void MathPhysicsEngine::RemoveRigidBody(RigidBody* r)
{
	rigidBodies.erase(std::remove(rigidBodies.begin(), rigidBodies.end(), r), rigidBodies.end());
	//particleForceRegistry->RemoveParticle(p);
	/*
	for (int i = contactGenerators.size() - 1; i > -1; i--) {

		if (ParticleLink* link = dynamic_cast<ParticleLink*>(contactGenerators.at(i))) {
			if (link->particle[0] == p || link->particle[1] == p) {

				delete link;
				auto iterator = contactGenerators.begin() + i;
				contactGenerators.erase(iterator);
			}
		}
	}*/
}

unsigned MathPhysicsEngine::GenerateContacts()
{
	//TODO OPTIMIZE
	for (ParticleContact* p : particlesContact) delete p;
	particlesContact.clear();
	
	for (ParticleContactGenerator* g : contactGenerators) {
		unsigned used = g->AddContact(particlesContact, maxContactNumber);

		if (particlesContact.size() >= maxContactNumber) break;
	}

	return static_cast<unsigned int>(particlesContact.size());
}

void MathPhysicsEngine::Init()
{
	particleForceRegistry = new ParticleForceRegistry();
	contactGenerators.push_back(new NaiveParticleContactGenerator(1.0f, &particles));
}

void MathPhysicsEngine::Update(double t,float frameTime)
{
	UpdateParticles(frameTime, t);
	
	UpdateRigidBodies(frameTime, t);

}

void MathPhysicsEngine::UpdateParticles(float frameTime, double t)
{
	//Generates Forces
	UpdateParticlesSumForces(frameTime);

	//Integrate
	for (Particle* p : particles) {
		p->SemiImpliciteEulerIntegration(t, (double)frameTime);
	}

	//Check every contact generator to get the current frame contact list
	unsigned usedContacts = GenerateContacts();

	//Solve the contacts
	if (usedContacts) {
		//number of iteration = double contacts number 
		contactResolver.SetIterationNumber(usedContacts * 2);
		contactResolver.ResolveContacts(particlesContact, frameTime);
	}
}

void MathPhysicsEngine::UpdateRigidBodies(float frameTime, double t)
{
	//Generates Forces
	//UpdateParticlesSumForces(frameTime);

	//Integrate
	for (RigidBody* r : rigidBodies) {
		r->Integrate(t, (double)frameTime);
	}

	//Check every contact generator to get the current frame contact list
	//unsigned usedContacts = GenerateContacts();

	//Solve the contacts
	/*
	if (usedContacts) {
		//number of iteration = double contacts number 
		contactResolver.SetIterationNumber(usedContacts * 2);
		contactResolver.ResolveContacts(particlesContact, frameTime);
	}*/
}

void MathPhysicsEngine::Shutdown()
{
	delete this->particleForceRegistry;
}

void MathPhysicsEngine::RemoveParticle(Particle* p)
{
	particles.erase(std::remove(particles.begin(), particles.end(), p), particles.end()); 
	particleForceRegistry->RemoveParticle(p);

	for (int i = contactGenerators.size()-1; i >-1; i--) {

		if (ParticleLink* link = dynamic_cast<ParticleLink*>(contactGenerators.at(i))) {
			if (link->particle[0] == p || link->particle[1] == p) {

				delete link;
				auto iterator = contactGenerators.begin() + i;
				contactGenerators.erase(iterator);
			}
		}
	}

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

void MathPhysicsEngine::UpdateParticlesSumForces(float frameTime)
{

	for (Particle* p : particles) {
		p->ClearSumForce();
	}

	particleForceRegistry->UpdateForce(frameTime);

}
MathPhysicsEngine* MathPhysicsEngine::singleton_ = nullptr;
