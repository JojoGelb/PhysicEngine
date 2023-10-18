#include "ConstantForce.h"

void ConstantForce::UpdateForce(Particle* particle, float duration)
{
	particle->AddForce(m_constantForce);

}


ConstantForce::ConstantForce(const Vector3D m_constantForce)
	: m_constantForce(m_constantForce)
{
}
