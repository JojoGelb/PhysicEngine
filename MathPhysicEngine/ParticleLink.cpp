#include "ParticleLink.h"

float ParticleLink::CurrentLength() const
{
    Vector3D relativePos = particle[0]->position - particle[1]->position;
    return relativePos.GetNorm();
}
