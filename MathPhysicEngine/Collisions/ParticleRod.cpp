#include "ParticleRod.h"

unsigned int ParticleRod::AddContact(std::vector<ParticleContact*>& contacts, unsigned int limit) const
{
    float currentLen = CurrentLength();

    if (currentLen == length) return 0;

    ParticleContact* contact = new ParticleContact();

    contact->particle[0] = particle[0];
    contact->particle[1] = particle[1];

    Vector3D normal = particle[1]->position - particle[0]->position;
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
