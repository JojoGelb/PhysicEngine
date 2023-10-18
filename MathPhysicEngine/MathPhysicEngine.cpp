#include "MathPhysicEngine.h"



MathPhysicsEngine::MathPhysicsEngine()
{
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

void MathPhysicsEngine::Init()
{
	particleForceRegistry = new ParticleForceRegistry();
}

void MathPhysicsEngine::Update(double t,float frameTime)
{
	UpdateSumForces(frameTime);

	for (Particle * p : particles) {
		p->SemiImpliciteEulerIntegration(t, (double)frameTime);
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
