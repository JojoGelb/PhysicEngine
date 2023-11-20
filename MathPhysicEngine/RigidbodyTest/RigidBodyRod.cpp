#include "RigidBodyRod.h"

unsigned int RigidBodyRod::AddContact(std::vector<RigidBodyContactTest*>& contacts, unsigned int limit) const
{
    float currentLen = CurrentLength();

    if (currentLen == length) return 0;

    RigidBodyContactTest* contact = new RigidBodyContactTest();

    contact->rigidbody[0] = rigidBody[0];
    contact->rigidbody[1] = rigidBody[1];

    Vector3D normal = rigidBody[1]->position - rigidBody[0]->position;
    normal.Normalize();

    if (currentLen > length) {

        contact->contactNormal = normal;
        contact->penetration = currentLen - length;

    }
    else {

        contact->contactNormal = normal * -1;
        contact->penetration = length - currentLen;

    }

    contact->restitution = 0;

    contacts.push_back(contact);

    return 1;
}
