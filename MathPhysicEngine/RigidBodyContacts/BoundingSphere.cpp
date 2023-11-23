#include "BoundingSphere.h"


BoundingSphere::BoundingSphere(const BoundingSphere &one, const BoundingSphere &two) {
    Vector3D centerOffset = two.center - one.center;
    float distance = centerOffset.GetSquareNorm();
    float radiusDiff = two.radius - one.radius;

    // Check if the larger sphere encloses the small one
    if (radiusDiff >= distance)
    {
        if (one.radius > two.radius)
        {
            center = one.center;
            radius = one.radius;
        }
        else
        {
            center = two.center;
            radius = two.radius;
        }
    }

    // Otherwise we need to work with partially
    // overlapping spheres
    else
    {
        float distance = centerOffset.GetNorm();
        // Find the center of the new sphere, and its radius
        radius = (distance + one.radius + two.radius) * ((float)0.5);

        // The new center is based on one’s center, moved towards
        // two’s center by an ammount proportional to the spheres’
        // radii.
        center = one.center;
        if (distance > 0)
        {
            center += centerOffset * ((radius - one.radius) / distance);
        }
    }
}


bool BoundingSphere::overlaps(const BoundingSphere *other) const
{
    float distanceSquared = (center - other->center).GetSquareNorm();
    return distanceSquared < (radius+other->radius)*(radius+other->radius);
}

/*float BoundingSphere::GetSize() const{
    return radius;
}*/
