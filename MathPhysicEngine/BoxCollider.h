#pragma once

#include "Collider.h"

struct Vector3D;

class BoxCollider: public Collider {

public:
	BoxCollider(Vector3D* _position, float _length) :Collider(_position), length(_length) { };

	//size of a side of the cube
	float length;


	// Hérité via Collider
	bool CollideWith(Collider* otherCollider) override;

};