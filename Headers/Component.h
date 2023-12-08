#pragma once

#include "../Sources/Transform.h"

class GameObject;

class Component {

public:
	
	GameObject* gameObject;
	Transform* transform;
	virtual void Start() = 0;
	virtual void Update() = 0;
	virtual void Shutdown() = 0;
};