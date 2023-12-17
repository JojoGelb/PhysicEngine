#include "MathPhysicEngine.h"
#include <iostream>
#include "Collisions/NaiveParticleContactGenerator.h"
#include "../Sources/GameObject.h"


MathPhysicsEngine::MathPhysicsEngine() : grid(2.9f)
{
	narrowCollisionDetector = NarrowCollisionDetector();
}

MathPhysicsEngine::~MathPhysicsEngine()
{
	for (ParticleContact* p : particlesContact) {
		delete p;
	}

	for (RigidBodyContact* p : rigidbodiesContact) {
		delete p;
	}

	delete particleForceRegistry;
	delete rigidBodyForceRegistry;

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
	rigidBodyForceRegistry->RemoveRigidBody(r);
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

void MathPhysicsEngine::Init()
{
	particleForceRegistry = new ParticleForceRegistry();
	rigidBodyForceRegistry = new RigidBodyForceRegistry();
	
	contactGenerators.push_back(new NaiveParticleContactGenerator(1.0f, &particles));
}

void MathPhysicsEngine::Update(double t,double frameTime)
{
	if (pause) return;
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

	//Calculate potential collisions : Broad Phase
	potentialCollision.clear();
	potentialCollision = grid.GetPotentialCollisions(rigidBodies);

	if(potentialCollision.size() <= 0) {
		return;
	}
	
	//std::cout << "Potential Collisions: " << potentialCollision.size() << std::endl;

	//Calculate collisions : Narrow Phase
	//std::vector<CollisionData*> collisionsData;

	//TODO: Clean out precedent rbContacts array values before adding new ones

	//TODO: Clean out precedent rbContacts array values before adding new ones
	narrowCollisionDetector.DetectCollisions(potentialCollision, rigidbodiesContact);

	if (rigidbodiesContact.size() > 0) {

		std::cout << "*******************new rigidbody update run *******************\n";

		for (int i = 0; i < rigidbodiesContact.size(); i++) {
			//std::cout << rigidbodiesContact.at(i)->rigidbody[0]->gameObject->GetName() << " - " << rigidbodiesContact.at(i)->rigidbody[1]->gameObject->GetName() << "\n";
			std::cout << "Penetration: " << rigidbodiesContact.at(i)->penetration << "\n";
			std::cout << "Normal: " << rigidbodiesContact.at(i)->contactNormal << "\n";
		}
		pause = true;
	}

	//Collision Resolution
	if(rigidbodiesContact.size() > 0) {
		std::cout << "Collisions: " << rigidbodiesContact.size() << std::endl;
		rigidbodyContactResolver.SetIterationNumber(rigidbodiesContact.size()/**2*/);
		rigidbodyContactResolver.ResolveContacts(rigidbodiesContact, frameTime);
	}

	//clean memory (delete every collisionData)
}

void MathPhysicsEngine::Shutdown()
{
	delete this->particleForceRegistry;
	delete this->rigidBodyForceRegistry;
}



void MathPhysicsEngine::SetFinalStates(const double alpha)
{
	if (pause) return;
	for (Particle* p : particles) {
		p->finalState = p->currentState * alpha + p->previousState * (1.0 - alpha);
		p->transform->position = p->finalState.position;
	}

	for (RigidBody* r : rigidBodies) {
		r->finalState = r->currentState * alpha + r->previousState * (1.0 - alpha);
		r->transform->transformMatrix = r->finalState.transformMatrix;
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
