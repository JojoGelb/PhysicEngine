#pragma once
#include<vector>
#include "Particule.h"
#include <GameObject.h>
struct ParticulesData
{
	std::vector<Particule> Particules = {};
	std::vector<GameObject> gameObjects = {};

};