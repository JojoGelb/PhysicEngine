#include "RigidBodyForceRegistry.h"
#include <iostream>

void RigidBodyForceRegistry::UpdateForce(float duration)
{
	for (RigidbodyEntry particleForceEntry : m_registry)
	{
		particleForceEntry.forceGenerator->UpdateForce(particleForceEntry.rigidBody, duration);
	}
}

void RigidBodyForceRegistry::AddForce(RigidBody* rigidBody, RigidBodyForceGenerator* forceGenerator)
{
	m_registry.push_back(RigidBodyForceRegistry::RigidbodyEntry(rigidBody, forceGenerator));
}

void RigidBodyForceRegistry::DeleteForce(RigidBody* rigidBody, RigidBodyForceGenerator* forceGenerator)
{
	for (int i = 0; i < m_registry.size(); i++) {
		if(m_registry.at(i).rigidBody == rigidBody) {
			if (m_registry.at(i).forceGenerator = forceGenerator) {

				m_registry.erase(m_registry.begin() + i);
			}

		}
	}
}

void RigidBodyForceRegistry::RemoveRigidBody(RigidBody* rigidBody)
{
	for (int i = 0; i < m_registry.size(); i++) {
	
		if (m_registry.at(i).rigidBody == rigidBody) {
			
			auto iterator = m_registry.begin() + i;
			m_registry.erase(iterator);
			i--;
			std::cout << "";
		}
	
	}
}

RigidBodyForceRegistry::RigidbodyEntry::RigidbodyEntry(RigidBody* rigidBody, RigidBodyForceGenerator* forceGenerator) : rigidBody(rigidBody), forceGenerator(forceGenerator){}
