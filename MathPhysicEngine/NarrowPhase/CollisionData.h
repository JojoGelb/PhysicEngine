#pragma once
#include "Primitives/CollisionPrimitive.h"
#include "../RigidbodyTest/RigidBodyContact.h"

/**
* A helper structure that contains information for the detector to use
* in building its contact data.
*/
struct CollisionData
{
    /** Holds the contact array to write into. */
    RigidBodyContact *contacts;
    /** Holds the maximum number of contacts the array can take. */
    unsigned contactsLeft;
};

void detectContacts(const CollisionPrimitive &firstPrimitive,
const CollisionPrimitive &secondPrimitive,
CollisionData *data);