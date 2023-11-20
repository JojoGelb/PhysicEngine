#include "MathPhysicEngine.h"
#include <iostream>
#include "Collisions/NaiveParticleContactGenerator.h"
#include "RigidbodyTest/RigidBodyLink.h"
#include "RigidbodyTest/NaiveRigidBodyContactGenerator.h"

MathPhysicsEngine::MathPhysicsEngine()
{
}

MathPhysicsEngine::~MathPhysicsEngine()
{
	for (ParticleContact* p : particlesContact) {
		delete p;
	}

	for (RigidBodyContactTest* p : rigidbodyContact) {
		delete p;
	}

	delete particleForceRegistry;
	delete rigidBodyForceRegistry;

	for (ParticleContactGenerator* p : contactGenerators) {
		delete p;
	}

	for (RigidbodyContactGeneratorTest* p : rigidbodyContactGenerator) {
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
	rigidBodyForceRegistry->RemoveRigidBody(r);
	
	for (int i = rigidbodyContactGenerator.size() - 1; i > -1; i--) {

		if (RigidBodyLink* link = dynamic_cast<RigidBodyLink*>(rigidbodyContactGenerator.at(i))) {
			if (link->rigidBody[0] == r || link->rigidBody[1] == r) {

				delete link;
				auto iterator = rigidbodyContactGenerator.begin() + i;
				rigidbodyContactGenerator.erase(iterator);
			}
		}
	}
}

void MathPhysicsEngine::RemoveParticle(Particle* p)
{
	particles.erase(std::remove(particles.begin(), particles.end(), p), particles.end());
	particleForceRegistry->RemoveParticle(p);

	for (int i = contactGenerators.size() - 1; i > -1; i--) {

		if (ParticleLink* link = dynamic_cast<ParticleLink*>(contactGenerators.at(i))) {
			if (link->particle[0] == p || link->particle[1] == p) {

				delete link;
				auto iterator = contactGenerators.begin() + i;
				contactGenerators.erase(iterator);
			}
		}
	}

}

unsigned MathPhysicsEngine::GenerateParticleContacts()
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

unsigned MathPhysicsEngine::GenerateRigidBodyContacts()
{
	//TODO OPTIMIZE
	for (RigidBodyContactTest* p : rigidbodyContact) delete p;
	rigidbodyContact.clear();

	for (RigidbodyContactGeneratorTest* g : rigidbodyContactGenerator) {
		unsigned used = g->AddContact(rigidbodyContact, maxContactNumber);

		if (rigidbodyContact.size() >= maxContactNumber) break;
	}

	return static_cast<unsigned int>(rigidbodyContact.size());
}

void MathPhysicsEngine::Init()
{
	particleForceRegistry = new ParticleForceRegistry();
	rigidBodyForceRegistry = new RigidBodyForceRegistry();
	
	contactGenerators.push_back(new NaiveParticleContactGenerator(1.0f, &particles));
	rigidbodyContactGenerator.push_back(new NaiveRigidBodyContactGenerator(1.0f, &rigidBodies));
}

void MathPhysicsEngine::Update(double t,double frameTime)
{
	UpdateParticles(frameTime, t);
	
	UpdateRigidBodies(frameTime, t);

}

void MathPhysicsEngine::UpdateParticles(double frameTime, double t)
{
	//Generates Forces
	UpdateParticlesSumForces(frameTime);

	//Integrate
	for (Particle* p : particles) {
		p->SemiImpliciteEulerIntegration(t, frameTime);
	}

	//Check every contact generator to get the current frame contact list
	unsigned usedContacts = GenerateParticleContacts();

	//Solve the contacts
	if (usedContacts) {
		//number of iteration = double contacts number 
		particleContactResolver.SetIterationNumber(usedContacts * 2);
		particleContactResolver.ResolveContacts(particlesContact, frameTime);
	}
}

void MathPhysicsEngine::UpdateRigidBodies(double frameTime, double t)
{
	//Generates Forces
	//UpdateParticlesSumForces(frameTime);

	//Integrate
	for (RigidBody* r : rigidBodies) {
		r->Integrate(t, frameTime);
	}

	//Check every contact generator to get the current frame contact list
	unsigned usedContacts = GenerateRigidBodyContacts();

	//Solve the contacts
	
	if (usedContacts) {
		//number of iteration = double contacts number 
		rigidbodyContactResolver.SetIterationNumber(usedContacts * 2);
		rigidbodyContactResolver.ResolveContacts(rigidbodyContact, frameTime);
	}
}

void MathPhysicsEngine::Shutdown()
{
	delete this->particleForceRegistry;
	delete this->rigidBodyForceRegistry;
}



void MathPhysicsEngine::SetFinalStates(const double alpha)
{
	for (Particle* p : particles) {
		p->finalState = p->currentState * alpha + p->previousState * (1.0 - alpha);
	}

	for (RigidBody* r : rigidBodies) {
		r->finalState = r->currentState * alpha + r->previousState * (1.0 - alpha);
	}
}

ParticleForceRegistry* MathPhysicsEngine::GetParticleForceRegistry()
{
	return this->particleForceRegistry;
}

RigidBodyForceRegistry* MathPhysicsEngine::GetRigidBodyForceRegistry()
{
	return this->rigidBodyForceRegistry;
}

void MathPhysicsEngine::UpdateParticlesSumForces(float frameTime)
{

	for (Particle* p : particles) {
		p->ClearSumForce();
	}

	for (RigidBody* r : rigidBodies) {
		r->ClearAccumulator();
	}	
	
	particleForceRegistry->UpdateForce(frameTime);
	rigidBodyForceRegistry->UpdateForce(frameTime);

}
MathPhysicsEngine* MathPhysicsEngine::singleton_ = nullptr;
