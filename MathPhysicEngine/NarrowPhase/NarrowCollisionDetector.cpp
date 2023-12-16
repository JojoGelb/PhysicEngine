#include "NarrowCollisionDetector.h"

#include <complex>
#include <iostream>
#include <cassert>

class Box;

unsigned NarrowCollisionDetector::SphereAndSphere(const Sphere& sphere1, const Sphere& sphere2,
                                                  RigidBodyContact* rigidbodyContact) const
{
    
    //Get the position of the spheres
    const Vector3D position1 = sphere1.GetPosition();
    const Vector3D position2 = sphere2.GetPosition();

    //Calculate the distance between the spheres
    const Vector3D midline = position1 - position2;
    const double distance = midline.GetNorm();

    //Verify if the spheres are colliding
    if(distance <= 0.0f || distance >= sphere1.radius + sphere2.radius)
        return 0;

    //CONTACT GENERATION
    
    //Calculate the normal of the collision
    const Vector3D normal = midline * (1.0f / distance);

    //Create the contact
    //RigidBodyContact* contact = collisionData->contacts;
    rigidbodyContact->contactNormal = normal;
    rigidbodyContact->contactPoint = position1 + midline * 0.5f;
    rigidbodyContact->penetration = sphere1.radius + sphere2.radius - distance;
    rigidbodyContact->rigidbody[0] = sphere1.rigidBody;
    rigidbodyContact->rigidbody[1] = sphere2.rigidBody;
    rigidbodyContact->restitution = 0.9f; //TODO use true restitution
    //TODO add friction

    return 1;
}

unsigned NarrowCollisionDetector::SphereAndBox(const Sphere& sphere, const Box& box,
    RigidBodyContact* rigidbodyContact) const
{
    // Transform the center of the sphere into box coordinates
    Vector3D center = sphere.GetPosition();
    Vector3D relCenter = box.transform->transformInverse(center);

    // Early-out check to see if we can exclude the contact.
    if (std::abs(relCenter.x) - sphere.radius > box.halfSize.x ||
   std::abs(relCenter.y) - sphere.radius > box.halfSize.y ||
    std::abs(relCenter.z) - sphere.radius > box.halfSize.z)
    {
        return 0;
    }

    //CONTACT GENERATION

    Vector3D closestPt(0,0,0);
    double dist;
    // Clamp each coordinate to the box.
    dist = relCenter.x;
    if (dist > box.halfSize.x) dist = box.halfSize.x;
    if (dist < -box.halfSize.x) dist = -box.halfSize.x;
    closestPt.x = dist;
    dist = relCenter.y;
    if (dist > box.halfSize.y) dist = box.halfSize.y;
    if (dist < -box.halfSize.y) dist = -box.halfSize.y;
    closestPt.y = dist;
    dist = relCenter.z;
    if (dist > box.halfSize.z) dist = box.halfSize.z;
    if (dist < -box.halfSize.z) dist = -box.halfSize.z;
    closestPt.z = dist;
    // Check we’re in contact.
    dist = (closestPt - relCenter).GetSquareNorm();
    if (dist > sphere.radius * sphere.radius) return 0;
    // Compile the contact.
    Vector3D closestPtWorld = box.transform->TransformPosition(closestPt);


    //RigidBodyContact* contact = collisionData->contacts;
    rigidbodyContact->contactNormal = (center - closestPtWorld);
    rigidbodyContact->contactNormal.Normalize();
    rigidbodyContact->contactPoint = closestPtWorld;
    rigidbodyContact->penetration = sphere.radius - sqrt(dist);
    // Write the appropriate data.
    rigidbodyContact->rigidbody[0] = box.rigidBody;
    rigidbodyContact->rigidbody[1] = sphere.rigidBody;
    rigidbodyContact->restitution = 1.0f;//TODO use true restitution
    //TODO add friction

    return 1;
}

static inline double penetrationOnAxis(
    const Box& one,
    const Box& two,
    const Vector3D& axis,
    const Vector3D& toCentre
)
{
    // Project the half-size of one onto axis
    double oneProject = one.TransformToAxis(axis);
    double twoProject = two.TransformToAxis(axis);

    // Project this onto the axis
    double distance = abs(toCentre.DotProduct(axis));


    // Return the overlap (i.e. positive indicates
    // overlap, negative indicates separation).
    return oneProject + twoProject - distance;
}


static inline bool tryAxis(
    const Box& one,
    const Box& two,
    Vector3D axis,
    const Vector3D& toCentre,
    unsigned index,

    // These values may be updated
    double& smallestPenetration,
    unsigned& smallestCase
)
{
    // Make sure we have a normalized axis, and don't check almost parallel axes
    if (axis.GetSquareNorm() < 0.0001) return true;
    axis.Normalize();

    double penetration = penetrationOnAxis(one, two, axis, toCentre);

    if (penetration < 0) return false;
    if (penetration < smallestPenetration) {
        smallestPenetration = penetration;
        smallestCase = index;
    }
    return true;
}

/*
 * This function checks if the two boxes overlap
 * along the given axis, returning the ammount of overlap.
 * The final parameter toCentre
 * is used to pass in the vector between the boxes centre
 * points, to avoid having to recalculate it each time.
 */


static inline Vector3D contactPoint(
    const Vector3D& pOne,
    const Vector3D& dOne,
    double oneSize,
    const Vector3D& pTwo,
    const Vector3D& dTwo,
    double twoSize,

    // If this is true, and the contact point is outside
    // the edge (in the case of an edge-face contact) then
    // we use one's midpoint, otherwise we use two's.
    bool useOne)
{
    Vector3D toSt, cOne, cTwo;
    double dpStaOne, dpStaTwo, dpOneTwo, smOne, smTwo;
    double denom, mua, mub;

    smOne = dOne.GetSquareNorm();
    smTwo = dTwo.GetSquareNorm();
    dpOneTwo = dTwo.DotProduct(dOne);

    toSt = pOne - pTwo;
    dpStaOne = dOne.DotProduct(toSt);
    dpStaTwo = dTwo.DotProduct(toSt);

    denom = smOne * smTwo - dpOneTwo * dpOneTwo;

    // Zero denominator indicates parrallel lines
    if (abs(denom) < 0.0001f) {
        return useOne ? pOne : pTwo;
    }

    mua = (dpOneTwo * dpStaTwo - smTwo * dpStaOne) / denom;
    mub = (smOne * dpStaTwo - dpOneTwo * dpStaOne) / denom;

    // If either of the edges has the nearest point out
    // of bounds, then the edges aren't crossed, we have
    // an edge-face contact. Our point is on the edge, which
    // we know from the useOne parameter.
    if (mua > oneSize ||
        mua < -oneSize ||
        mub > twoSize ||
        mub < -twoSize)
    {
        return useOne ? pOne : pTwo;
    }
    else
    {
        cOne = pOne + dOne * mua;
        cTwo = pTwo + dTwo * mub;

        return cOne * 0.5 + cTwo * 0.5;
    }
}


void fillPointFaceBoxBox(
    const Box& one,
    const Box& two,
    const Vector3D& toCentre,
    RigidBodyContact* rigidbodyContact,
    unsigned best,
    double pen
)
{
    // This method is called when we know that a vertex from
    // box two is in contact with box one.

    // We know which axis the collision is on (i.e. best),
    // but we need to work out which of the two faces on
    // this axis.
    Vector3D normal = one.getAxis(best);
    if (one.getAxis(best).DotProduct(toCentre) > 0)
    {
        normal = normal * -1.0f;
    }

    // Work out which vertex of box two we're colliding with.
    // Using toCentre doesn't work!
    Vector3D vertex = two.halfSize;
    if (two.getAxis(0).DotProduct(normal) < 0) vertex.x = -vertex.x;
    if (two.getAxis(1).DotProduct(normal) < 0) vertex.y = -vertex.y;
    if (two.getAxis(2).DotProduct(normal) < 0) vertex.z = -vertex.z;

    // Create the contact data
    rigidbodyContact->contactNormal = normal;
    rigidbodyContact->penetration = pen;
    rigidbodyContact->contactPoint = *two.transform * vertex;
    rigidbodyContact->rigidbody[0] = one.rigidBody;
    rigidbodyContact->rigidbody[1] = two.rigidBody;
    rigidbodyContact->restitution = 1.0f;//TODO use true restitution
    //TODO add friction

}

unsigned NarrowCollisionDetector::boxAndPoint(
    const Box& box,
    const Vector3D& point,
    RigidBodyContact* rigidbodyContact
) const
{
    // Transform the point into box coordinates.
    Vector3D relPoint = box.transform->transformInverse(point);
    Vector3D normal;

    // Check each axis, looking for the axis on which the
    // penetration is least deep.
    double min_depth = box.halfSize.x - abs(relPoint.x);
    if (min_depth < 0) return 0;
    normal = box.getAxis(0) * ((relPoint.x < 0) ? -1 : 1);
    double depth = box.halfSize.y - abs(relPoint.y);
    if (depth < 0) return 0;
    else if (depth < min_depth)
    {
        min_depth = depth;
        normal = box.getAxis(1) * ((relPoint.y < 0) ? -1 : 1);
    }
    depth = box.halfSize.z - abs(relPoint.z);
    if (depth < 0) return 0;
    else if (depth < min_depth)
    {
        min_depth = depth;
        normal = box.getAxis(2) * ((relPoint.z < 0) ? -1 : 1);
    }
    // Compile the contact.
    rigidbodyContact->contactNormal = normal;
    rigidbodyContact->contactPoint = point;
    rigidbodyContact->penetration = min_depth;
    // Write the appropriate data.
    rigidbodyContact->rigidbody[0] = box.rigidBody;
    // Note that we don’t know what rigid body the point
    // belongs to, so we just use NULL. Where this is called
    // this value can be left, or filled in.
    rigidbodyContact->rigidbody[1] = NULL;
    rigidbodyContact->restitution = 1.0f;//TODO use true restitution
    //TODO add friction

    return 1;
}


// This preprocessor definition is only used as a convenience
// in the boxAndBox contact generation method.
#define CHECK_OVERLAP(axis, index) \
    if (!tryAxis(one, two, (axis), toCentre, (index), pen, best)) return 0;


unsigned NarrowCollisionDetector::BoxAndBox(const Box& one, const Box& two,
    RigidBodyContact* rigidbodyContact) const {


    //if (!IntersectionTests::boxAndBox(one, two)) return 0;

       // Find the vector between the two centres
    Vector3D toCentre = two.GetPosition() - one.GetPosition();

    // We start assuming there is no contact
    double pen = std::numeric_limits<double>::max();
    unsigned best = 0xffffff;

    // Now we check each axes, returning if it gives us
    // a separating axis, and keeping track of the axis with
    // the smallest penetration otherwise.
    CHECK_OVERLAP(one.getAxis(0), 0);
    CHECK_OVERLAP(one.getAxis(1), 1);
    CHECK_OVERLAP(one.getAxis(2), 2);

    CHECK_OVERLAP(two.getAxis(0), 3);
    CHECK_OVERLAP(two.getAxis(1), 4);
    CHECK_OVERLAP(two.getAxis(2), 5);

    // Store the best axis-major, in case we run into almost
    // parallel edge collisions later
    unsigned bestSingleAxis = best;

    CHECK_OVERLAP(one.getAxis(0) % two.getAxis(0), 6);
    CHECK_OVERLAP(one.getAxis(0) % two.getAxis(1), 7);
    CHECK_OVERLAP(one.getAxis(0) % two.getAxis(2), 8);
    CHECK_OVERLAP(one.getAxis(1) % two.getAxis(0), 9);
    CHECK_OVERLAP(one.getAxis(1) % two.getAxis(1), 10);
    CHECK_OVERLAP(one.getAxis(1) % two.getAxis(2), 11);
    CHECK_OVERLAP(one.getAxis(2) % two.getAxis(0), 12);
    CHECK_OVERLAP(one.getAxis(2) % two.getAxis(1), 13);
    CHECK_OVERLAP(one.getAxis(2) % two.getAxis(2), 14);

    // Make sure we've got a result.
    assert(best != 0xffffff);

    // We now know there's a collision, and we know which
    // of the axes gave the smallest penetration. We now
    // can deal with it in different ways depending on
    // the case.
    if (best < 3)
    {
        // We've got a vertex of box two on a face of box one.
        fillPointFaceBoxBox(one, two, toCentre, rigidbodyContact, best, pen);
       // data->addContacts(1);
        return 1;
    }
    else if (best < 6)
    {
        // We've got a vertex of box one on a face of box two.
        // We use the same algorithm as above, but swap around
        // one and two (and therefore also the vector between their
        // centres).
        fillPointFaceBoxBox(two, one, toCentre * -1.0f, rigidbodyContact, best - 3, pen);
       // data->addContacts(1);
        return 1;
    }
    else
    {
        // We've got an edge-edge contact. Find out which axes
        best -= 6;
        unsigned oneAxisIndex = best / 3;
        unsigned twoAxisIndex = best % 3;
        Vector3D oneAxis = one.getAxis(oneAxisIndex);
        Vector3D twoAxis = two.getAxis(twoAxisIndex);
        Vector3D axis = oneAxis % twoAxis;
        axis.Normalize();

        // The axis should point from box one to box two.
        if (axis.DotProduct(toCentre) > 0) axis = axis * -1.0f;

        // We have the axes, but not the edges: each axis has 4 edges parallel
        // to it, we need to find which of the 4 for each object. We do
        // that by finding the point in the centre of the edge. We know
        // its component in the direction of the box's collision axis is zero
        // (its a mid-point) and we determine which of the extremes in each
        // of the other axes is closest.
        Vector3D ptOnOneEdge = one.halfSize;
        Vector3D ptOnTwoEdge = two.halfSize;
        for (unsigned i = 0; i < 3; i++)
        {
            if (i == oneAxisIndex) ptOnOneEdge.SetAtIndex(i, 0);
            else if (one.getAxis(i).DotProduct(axis) > 0) ptOnTwoEdge.SetAtIndex(i,-ptOnTwoEdge.GetAtIndex(i));

            if (i == twoAxisIndex) ptOnTwoEdge.SetAtIndex(i, 0);
			else if (two.getAxis(i).DotProduct(axis) < 0) ptOnTwoEdge.SetAtIndex(i, -ptOnTwoEdge.GetAtIndex(i));
        }

        // Move them into world coordinates (they are already oriented
        // correctly, since they have been derived from the axes).
        ptOnOneEdge = *one.transform * ptOnOneEdge;
        ptOnTwoEdge = *two.transform * ptOnTwoEdge;

        // So we have a point and a direction for the colliding edges.
        // We need to find out point of closest approach of the two
        // line-segments.
        Vector3D vertex = contactPoint(
            ptOnOneEdge, oneAxis, one.halfSize.GetAtIndex(oneAxisIndex),
            ptOnTwoEdge, twoAxis, two.halfSize.GetAtIndex(twoAxisIndex),
            bestSingleAxis > 2
        );

        // We can fill the contact.

        rigidbodyContact->penetration = pen;
        rigidbodyContact->contactNormal = axis;
        rigidbodyContact->contactPoint = vertex;
        rigidbodyContact->rigidbody[0] = one.rigidBody;
        rigidbodyContact->rigidbody[1] = two.rigidBody;
        rigidbodyContact->restitution = 1.0f;//TODO use true restitution
        //TODO add friction
        return 1;
    }
    return 0;
}
#undef CHECK_OVERLAP







bool NarrowCollisionDetector::OverlapOnAxis(
const Box &one,
const Box &two,
const Vector3D &axis
)
{
    // Project the half-size of one onto axis.
    double oneProject = one.TransformToAxis(axis);
    double twoProject = two.TransformToAxis(axis);
    // Find the vector between the two centers.
    Vector3D toCenter = two.GetPosition() - one.GetPosition();
    // Project this onto the axis.
    double distance = abs(toCenter.DotProduct(axis));
    // Check for overlap.
    return (distance < oneProject + twoProject);
}

void NarrowCollisionDetector::DetectCollisions(const std::vector<PotentialCollision>& potentialCollisions,
    std::vector<RigidBodyContact*>& rigidbodiesContact) const
{
    //if sphere && sphere
    //do sphere && box detection

    rigidbodiesContact.clear();

    if (potentialCollisions.size() > 0)
    {
		

        for (PotentialCollision potentialCollision : potentialCollisions)
        {
            //Verify collisions and create contacts lists
            RigidBodyContact* rigidbodyContact =new RigidBodyContact();

			//if sphere && sphere
            if (potentialCollision.rigidBodies[0]->collisionPrimitive->shape == CollisionShape::SPHERE &&
                potentialCollision.rigidBodies[1]->collisionPrimitive->shape == CollisionShape::SPHERE)
            {
				auto i = SphereAndSphere(*((Sphere*)potentialCollision.rigidBodies[0]->collisionPrimitive),
                    *((Sphere*)potentialCollision.rigidBodies[1]->collisionPrimitive), rigidbodyContact);
			}
			//if sphere && box
            else if (potentialCollision.rigidBodies[0]->collisionPrimitive->shape == CollisionShape::SPHERE &&
                potentialCollision.rigidBodies[1]->collisionPrimitive->shape == CollisionShape::BOX)
            {
				//do sphere && box detection
                auto i = SphereAndBox(*((Sphere*)potentialCollision.rigidBodies[0]->collisionPrimitive),
                    					*((Box*)potentialCollision.rigidBodies[1]->collisionPrimitive), rigidbodyContact);
            }
            //if box && sphere
            else if (potentialCollision.rigidBodies[0]->collisionPrimitive->shape == CollisionShape::BOX &&
                potentialCollision.rigidBodies[1]->collisionPrimitive->shape == CollisionShape::SPHERE)
            {
				//do sphere && box detection
                auto i = SphereAndBox(*((Sphere*)potentialCollision.rigidBodies[1]->collisionPrimitive),
                    										*((Box*)potentialCollision.rigidBodies[0]->collisionPrimitive), rigidbodyContact);
            }
            //if box && box
            else if (potentialCollision.rigidBodies[0]->collisionPrimitive->shape == CollisionShape::BOX &&
                potentialCollision.rigidBodies[1]->collisionPrimitive->shape == CollisionShape::BOX)
            {
                //do box && box detection
                auto i = BoxAndBox(*((Box*)potentialCollision.rigidBodies[1]->collisionPrimitive),
                    *((Box*)potentialCollision.rigidBodies[0]->collisionPrimitive), rigidbodyContact);
            }


            if (rigidbodyContact->rigidbody[0] == nullptr) {
                delete rigidbodyContact;
                continue;
            }
			
            rigidbodyContact->restitution = RigidBody::contactRestitution;
            rigidbodiesContact.push_back(rigidbodyContact);
 
		}
       
	}

   
    
    //if sphere && box
    //do sphere && box detection
}
