#pragma once
#include <vector>
#include "RigidBodyContact.h"

class RigidbodyContactGeneratorTest
{
public:
	virtual unsigned int AddContact(std::vector<RigidBodyContact*>& contacts, unsigned int limit) const = 0;

};

