#include "ParticleCable.h"

unsigned int ParticleCable::AddContact(std::vector<ParticleContact*>& contacts, unsigned int limit) const
{
    float length = CurrentLength();

    if (length > maxLength) return 0;

    ParticleContact* contact = new ParticleContact();

    contact->particle[0] = particle[0];
    contact->particle[1] = particle[1];

    Vector3D normal = particle[1]->position - particle[0]->position;
    normal.Normalize();
    contact->contactNormal = normal;

    contact->penetration = length - maxLength;
    contact->restitution = restitution;

    contacts.push_back(contact);

    return 1;
}