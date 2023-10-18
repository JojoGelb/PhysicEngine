#include "BoxCollider.h"
#include "Vecteur3D.h"

bool BoxCollider::CollideWith(Collider* otherCollider)
{
    if (const BoxCollider* otherCube = dynamic_cast<const BoxCollider*>(otherCollider)) {
        
        //distance btw center of both cubes
        float dx = std::abs(position->x - otherCube->position->x);
        float dy = std::abs(position->y - otherCube->position->y);
        float dz = std::abs(position->z - otherCube->position->z);

        float halfLength = length / 2 + otherCube->length / 2;

        bool xIntersect = dx <= halfLength;
        bool yIntersect = dy <= halfLength;
        bool zIntersect = dz <= halfLength;

        return xIntersect && yIntersect && zIntersect;

    }


    return false;
}
