#include "RigidBodyCable.h"

unsigned int RigidBodyCable::AddContact(std::vector<RigidBodyContactTest*>& contacts, unsigned int limit) const
{
    float length = CurrentLength();

    if (length < maxLength) return 0;

    RigidBodyContactTest* contact = new RigidBodyContactTest();

    contact->rigidbody[0] = rigidBody[0];
    contact->rigidbody[1] = rigidBody[1];

    Vector3D normal = rigidBody[1]->position - rigidBody[0]->position;
    normal.Normalize();
    contact->contactNormal = normal;

    contact->penetration = length - maxLength;
    contact->restitution = restitution;

    contacts.push_back(contact);

    return 1;
}