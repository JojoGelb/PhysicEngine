#pragma once
#include "../Rigidbody/RigidBody.h"
/*

class that handle BHV tree

*/

struct PotentialContact
{
    RigidBody *body[2];
};

template <class BoundingVolumeClass>
class BVHNode
{
public:
    BVHNode *children[2];
    BVHNode *parent;

    /**
     * Deletes this node, removing it first from the hierarchy, along
     * with its associated rigid body and child nodes. This method
     * deletes the node and all its children (but obviously not the
     * rigid bodies). This also has the effect of deleting the sibling
     * of this node, and changing the parent node so that it contains
     * the data currently in that sibling. Finally it forces the
     * hierarchy above the current node to reconsider its bounding
     * volume.
     */
    ~BVHNode();

    // volume englobant
    BoundingVolumeClass volume;

    // Only for leaf, null otherwise
    RigidBody *body = nullptr;

    bool isLeaf() const;

    bool overlaps(const BVHNode<BoundingVolumeClass>* other) const;

    unsigned getPotentialContacts(PotentialContact* contacts, unsigned limit) const;

    unsigned getPotentialContactsWith(const BVHNode<BoundingVolumeClass>* other, PotentialContact* contacts, unsigned limit) const;
    
    void insert(RigidBody* body, const BoundingVolumeClass& volume);

    void recalculateBoundingVolume();
};