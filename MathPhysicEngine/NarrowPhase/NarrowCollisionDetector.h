#pragma once
#include <vector>

#include "CollisionData.h"
#include "../BroadPhase/Grid.h"
#include "Primitives/Box.h"
#include "Primitives/Sphere.h"

class NarrowCollisionDetector
{
public:
    unsigned SphereAndSphere(const Sphere& sphere1, const Sphere& sphere2, CollisionData *collisionData) const;
    unsigned SphereAndBox(const Sphere& sphere, const Box& box, CollisionData* collisionData) const;
    bool OverlapOnAxis(const Box& one, const Box& two, const Vector3D& axis);
    void DetectCollisions(const std::vector<PotentialCollision>& potentialCollisions, CollisionData* collisionData) const;
};
