#pragma once
#include<vector>
#include "Particle.h"
#include <GameObject.h>
struct ObjectData
{
	std::vector<std::string> names = {};
	std::vector<Particle> particles = {};
	std::vector<GameObject> gameObjects = {};

};