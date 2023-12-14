#pragma once
#include "Vecteur3D.h"
#include "Particle.h"
#include "Rigidbody/RigidBody.h"
#include <vector>
#include "Forces/ParticleForceRegistry.h"
#include "Collisions/ParticleContact.h"
#include "Collisions/ParticleContactResolver.h"
#include "Collisions/ParticleContactGenerator.h"
#include "Collisions/ParticleRod.h"
#include "Collisions/ParticleCable.h"
#include "Forces/RigidBodyForceRegistry.h"

#include "Forces/ParticuleForces/ParticleGravity.h"
#include "RigidbodyTest/RigidBodyContact.h"
#include "BroadPhase/Grid.h"
#include "NarrowPhase/NarrowCollisionDetector.h"
#include "RigidbodyCollisionResolution/RigidbodyContactResolver.h"

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


	void Update(double t, double frameTime);
	void Shutdown();

	//Particles management
	void AddParticle(Particle* p) { particles.push_back(p); }
	void RemoveParticle(Particle* p);
	void SetFinalStates(const double alpha);

	//RigidBodies management
	void AddRigidBody(RigidBody* r) { rigidBodies.push_back(r); }
	void RemoveRigidBody(RigidBody* r);

	
	unsigned GenerateParticleContacts();


	ParticleForceRegistry* GetParticleForceRegistry();
	RigidBodyForceRegistry* GetRigidBodyForceRegistry();

	bool pause = false;

private:

	void UpdateParticles(double frameTime, double t);

	void UpdateRigidBodies(double frameTime, double t);

	void UpdateParticlesSumForces(float frameTime);

	void Init();
	int maxContactNumber = 10;

	std::vector<RigidBody*> rigidBodies;

	std::vector<Particle*> particles;

	//Particle forces
	ParticleForceRegistry* particleForceRegistry;

	//Particle Contacts
	ParticleContactResolver particleContactResolver;
	std::vector<ParticleContactGenerator*> contactGenerators; 
	std::vector<ParticleContact*> particlesContact; 

	//Rigidbody Contacts
	RigidbodyContactResolver rigidbodyContactResolver;
	std::vector<RigidBodyContact*> rigidbodiesContact;

	//RigidBody forces
	RigidBodyForceRegistry* rigidBodyForceRegistry;

	Grid grid;

	NarrowCollisionDetector narrowCollisionDetector;

public:

	void TestCableCollisionSetup(Particle* a, Particle* b, float cableLen) {
		contactGenerators.push_back(new ParticleCable(a, b, cableLen, 1));
	}

	void TestRodCollisionSetup(Particle* a, Particle* b, float cableLen) {
		contactGenerators.push_back(new ParticleRod(a, b, cableLen));
	}

	ParticleGravity* particleGravity = new ParticleGravity({ 0.0f,-10.0f,0.0f });

};
