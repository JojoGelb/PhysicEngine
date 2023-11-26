#pragma once
#include "../Rigidbody/RigidBody.h"
#include <iostream>
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

    BVHNode(BVHNode *parent, const BoundingVolumeClass &volume, RigidBody *body = nullptr) : parent(parent), volume(volume), body(body)
    {
        children[0] = children[1] = nullptr;
    }

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
    ~BVHNode() { // If we don�t have a parent, then we ignore the sibling processing.
        if (parent != nullptr)
        {
            // Find our sibling.
            BVHNode<BoundingVolumeClass>* sibling;
            if (parent->children[0] == this)
                sibling = parent->children[1];
            else
                sibling = parent->children[0];
            // Write its data to our parent.
            parent->volume = sibling->volume;
            parent->body = sibling->body;
            parent->children[0] = sibling->children[0];
            parent->children[1] = sibling->children[1];
            // Delete the sibling (we blank its parent and
            // children to avoid processing/deleting them).
            sibling->parent = nullptr;
            sibling->body = nullptr;
            sibling->children[0] = nullptr;
            sibling->children[1] = nullptr;
            delete sibling;
            // Recalculate the parent�s bounding volume.
            parent->recalculateBoundingVolume();
        }
        // Delete our children (again we remove their parent data so
        // we don�t try to process their siblings as they are deleted).
        if (children[0] != nullptr)
        {
            children[0]->parent = nullptr;
            delete children[0];
        }
        if (children[1] != nullptr)
        {
            children[1]->parent = nullptr;
            delete children[1];
        }
    }

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

template <class BoundingVolumeClass>
unsigned BVHNode<BoundingVolumeClass>::getPotentialContactsWith(const BVHNode<BoundingVolumeClass>* other, PotentialContact* contacts, unsigned limit) const
{
    // Early-out if we don�t overlap or if we have no room
    // to report contacts.
    if (!overlaps(other) || limit == 0)
        return 0;
    // If we�re both at leaf nodes, then we have a potential contact.
    if (isLeaf() && other->isLeaf())
    {
        contacts->body[0] = body;
        contacts->body[1] = other->body;
        return 1;
    }
    // Determine which node to descend into. If either is
    // a leaf, then we descend the other. If both are branches,
    // then we use the one with the largest size.
    if (other->isLeaf() ||
        (!isLeaf() && volume->getSize() >= other->volume->getSize()))
    {
        // Recurse into ourself.
        unsigned count = children[0]->getPotentialContactsWith(
            other, contacts, limit);
        // Check whether we have enough slots to do the other side too.
        if (limit > count)
        {
            return count + children[1]->getPotentialContactsWith(
                other, contacts + count, limit - count);
        }
        else
        {
            return count;
        }
    }
    else
    {
        // Recurse into the other node.
        unsigned count = getPotentialContactsWith(
            other->children[0], contacts, limit);
        // Check whether we have enough slots to do the other side too.
        if (limit > count)
        {
            return count + getPotentialContactsWith(
                other->children[1], contacts + count, limit - count);
        }
        else
        {
            return count;
        }
    }
}


template <class BoundingVolumeClass>
void BVHNode<BoundingVolumeClass>::insert(
    RigidBody* newBody, const BoundingVolumeClass& newVolume)
{
    // If we are a leaf, then the only option is to spawn two
    // new children and place the new body in one.
    if (isLeaf())
    {
        // Child one is a copy of us.
        children[0] = new BVHNode<BoundingVolumeClass>(
            this, volume, body);
        // Child two holds the new body
        children[1] = new BVHNode<BoundingVolumeClass>(
            this, newVolume, newBody);
        // And we now loosen the body (we�re no longer a leaf).
        this->body = nullptr; // We need to recalculate our bounding volume.
        recalculateBoundingVolume();
    }
    // Otherwise we need to work out which child gets to keep
    // the inserted body. We give it to whoever would grow the
    // least to incorporate it.
    else
    {
        if (children[0]->volume.GetGrowth(newVolume) <
            children[1]->volume.GetGrowth(newVolume))
        {
            children[0]->insert(newBody, newVolume);
        }
        else
        {
            children[1]->insert(newBody, newVolume);
        }
    }
}

template <class BoundingVolumeClass>
void BVHNode<BoundingVolumeClass>::recalculateBoundingVolume()
{
    if (isLeaf()) return;
    volume = BoundingVolumeClass(children[0]->volume, children[1]->volume);
    if (parent) parent->recalculateBoundingVolume();
}

template <class BoundingVolumeClass>
bool BVHNode<BoundingVolumeClass>::isLeaf() const
{
    return body != nullptr;
}

template <class BoundingVolumeClass>
bool BVHNode<BoundingVolumeClass>::overlaps(const BVHNode<BoundingVolumeClass>* other) const
{
    return volume->overlaps(other->volume);
}

template <class BoundingVolumeClass>
unsigned BVHNode<BoundingVolumeClass>::getPotentialContacts(PotentialContact* contacts, unsigned limit) const
{
    // Early out if we don�t have the room for contacts, or
    // if we�re a leaf node.
    if (isLeaf() || limit == 0)
        return 0;
    // Get the potential contacts of one of our children with
    // the other.
    return children[0]->getPotentialContactsWith(
        children[1], contacts, limit);
}