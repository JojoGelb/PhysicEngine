#pragma once
#include<vector>
#include "../MathPhysicEngine/Particle.h"

#include <GameObject.h>
struct ObjectData
{
	std::vector<std::string> names = {};
	std::vector<Particle> particles = {};
	std::vector<GameObject> gameObjects = {};

};