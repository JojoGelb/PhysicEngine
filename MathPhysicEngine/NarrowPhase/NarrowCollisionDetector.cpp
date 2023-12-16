#include "NarrowCollisionDetector.h"

#include <complex>
#include <iostream>

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
    Vector3D toCenter = two.GetPosition().z - one.GetPosition().z;
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
                //std::cout << "Sphere and Sphere" << std::endl;
				//do sphere && sphere detection
				auto i = SphereAndSphere(*((Sphere*)potentialCollision.rigidBodies[0]->collisionPrimitive),
                    *((Sphere*)potentialCollision.rigidBodies[1]->collisionPrimitive), rigidbodyContact);
                //std::cout << i << std::endl;
			}
			//if sphere && box
            else if (potentialCollision.rigidBodies[0]->collisionPrimitive->shape == CollisionShape::SPHERE &&
                potentialCollision.rigidBodies[1]->collisionPrimitive->shape == CollisionShape::BOX)
            {
                //std::cout << "Sphere and Box" << std::endl;
				//do sphere && box detection
                auto i = SphereAndBox(*((Sphere*)potentialCollision.rigidBodies[0]->collisionPrimitive),
                    					*((Box*)potentialCollision.rigidBodies[1]->collisionPrimitive), rigidbodyContact);
				//std::cout << i << std::endl;
            }
            //if box && sphere
            else if (potentialCollision.rigidBodies[0]->collisionPrimitive->shape == CollisionShape::BOX &&
                potentialCollision.rigidBodies[1]->collisionPrimitive->shape == CollisionShape::SPHERE)
            {
                //std::cout << "Box and Sphere" << std::endl;
				//do sphere && box detection
                auto i = SphereAndBox(*((Sphere*)potentialCollision.rigidBodies[1]->collisionPrimitive),
                    										*((Box*)potentialCollision.rigidBodies[0]->collisionPrimitive), rigidbodyContact);
			
               // std::cout << i << std::endl;
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
