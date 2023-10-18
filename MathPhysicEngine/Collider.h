#pragma once

#include "Vecteur3D.h"

class Collider {
public:
	Collider(Vector3D* _position) :position(_position) {};
	virtual bool CollideWith(Collider* otherCollider) = 0;

	Vector3D* position;
};