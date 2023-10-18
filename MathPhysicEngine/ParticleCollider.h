#pragma once

#include "Collider.h"
#include "Vecteur3D.h"

class ParticleCollider : public Collider {

public:
	ParticleCollider(Vector3D* _position, float _radius) :Collider(_position), radius(_radius) { };

	//size of a side of the cube
	float radius;


	// Hérité via Collider
	bool CollideWith(Collider* otherCollider) override;

};