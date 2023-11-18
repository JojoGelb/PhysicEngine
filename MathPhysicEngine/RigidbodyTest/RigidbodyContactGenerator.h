#pragma once
#include <vector>
#include "RigidBodyContactTest.h"

class RigidbodyContactGeneratorTest
{
public:
	virtual unsigned int AddContact(std::vector<RigidBodyContactTest*>& contacts, unsigned int limit) const = 0;

};

