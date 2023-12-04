#pragma once
#include <unordered_map>
#include <vector>
#include "../Vecteur3D.h"
#include "../Rigidbody/RigidBody.h"


struct PotentialCollision {
    RigidBody* rigidBodies[2];
};

class Grid {
private:
    float _cellSize = 1.2f;
    //ease insert
    std::unordered_map<Vector3D, std::vector<RigidBody*>, Vector3D> rigidbodiesInCell;

    //Hold everyCells with rigidbodies inside
    std::vector<std::vector<RigidBody*>*> filledCells;

    void AddRigidBodyToGrid(Vector3D v, RigidBody* rb);

public:
    Grid(float cellSize);

    std::vector<PotentialCollision> GetPotentialCollisions(std::vector<RigidBody*> rigidbodies);
};