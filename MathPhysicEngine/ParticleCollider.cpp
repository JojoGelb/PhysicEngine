#include "ParticleCollider.h"

bool ParticleCollider::CollideWith(Collider* otherCollider)
{
    if (const ParticleCollider* other = dynamic_cast<const ParticleCollider*>(otherCollider)) {
        float distance = position->DistanceTo(*otherCollider->position);
        return distance < radius + other->radius;
    }

    return false;
}
