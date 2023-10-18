#pragma once
#include "ParticleContact.h"
#include <vector>

class ParticleContactGenerator {

public:

	virtual unsigned int AddContact(std::vector<ParticleContact*>& contacts, unsigned int limit) const = 0;

};