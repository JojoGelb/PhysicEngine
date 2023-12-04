#pragma once


#include "../../Rigidbody/RigidBody.h"
#include "../../Rigidbody/Matrix34.h"

class CollisionPrimitive
{
public:
    RigidBody *rigidBody;

    //Offset of the primitive from the rigidbody's center of mass
    Matrix34 offset;
    
    Vector3D GetPosition() const;
    void UpdateTransformMatrix();

    //result of the combination of the rigidbody's transform
    //and the offset of the primitive
    Matrix34 transform;


    
};
