#include "Grid.h"
#include "../Sources/Transform.h"
#include <iostream>

Grid::Grid(float cellSize) : _cellSize(cellSize)
{
}

std::vector<PotentialCollision> Grid::GetPotentialCollisions(std::vector<RigidBody*> rigidbodies)
{	
	std::vector<PotentialCollision> collision = std::vector<PotentialCollision>();
	
	filledCells.clear();
	rigidbodiesInCell.clear();

	for (int i = 0; i < rigidbodies.size(); i++) {
		
		float x1 = rigidbodies[i]->transform->position.x - rigidbodies[i]->boundingVolume->GetXSize();
		float x2 = rigidbodies[i]->transform->position.x + rigidbodies[i]->boundingVolume->GetXSize();

		float gridX1 = floorf(x1 / _cellSize);
		float gridX2 = floorf(x2 / _cellSize);

		float y1 = rigidbodies[i]->transform->position.y - rigidbodies[i]->boundingVolume->GetYSize();
		float y2 = rigidbodies[i]->transform->position.y + rigidbodies[i]->boundingVolume->GetYSize();

		float gridY1 = floorf(y1 / _cellSize);
		float gridY2 = floorf(y2 / _cellSize);

		float z1 = rigidbodies[i]->transform->position.z - rigidbodies[i]->boundingVolume->GetZSize();
		float z2 = rigidbodies[i]->transform->position.z + rigidbodies[i]->boundingVolume->GetZSize();

		float gridZ1 = floorf(z1 / _cellSize);
		float gridZ2 = floorf(z2 / _cellSize);

		/*std::cout << i << " " << 
			rigidbodies[i]->position << " " << 
			"GridX " << gridX1 << " " << gridX2 << " " <<
			"GridY " << gridY1 << " " << gridY2 << " " <<
			"GridZ " << gridZ1 << " " << gridZ2 <<
			
			" \n";*/

		bool onTwoZGrid = gridZ1 != gridZ2;
		bool onTwoYGrid = gridY2 != gridY1;
		bool onTwoXGrid = gridX1 != gridX2;

		Vector3D v = Vector3D(gridX1, gridY1, gridZ1);
		AddRigidBodyToGrid(v, rigidbodies[i]); //111

		if (onTwoXGrid) { //2+ possibilities
			v.x = gridX2;
			AddRigidBodyToGrid(v, rigidbodies[i]); //211

			if (onTwoZGrid) { //4+ possibilities 112 , 212
				v.z = gridZ2;
				v.x = gridX1;
				AddRigidBodyToGrid(v, rigidbodies[i]); //112
				
				v.x = gridX2;
				AddRigidBodyToGrid(v, rigidbodies[i]); //212


				if (onTwoYGrid) { // 8 possibilities: 222, 122, 121, 221
					v.y = gridY2;
					AddRigidBodyToGrid(v, rigidbodies[i]); //222

					v.x = gridX1;
					AddRigidBodyToGrid(v, rigidbodies[i]); //122

					v.z = gridZ1;
					AddRigidBodyToGrid(v, rigidbodies[i]); //121

					v.x = gridX2;
					AddRigidBodyToGrid(v, rigidbodies[i]); //221
				}
				
			}
			else { //2+ possibilities
				if (onTwoYGrid) { //4 possibilties 
					v.y = gridY2;
					AddRigidBodyToGrid(v, rigidbodies[i]); //221

					v.x = gridX1;
					AddRigidBodyToGrid(v, rigidbodies[i]); //121
				}
			}

		}
		else { // 1+ possibilities
		
			if (onTwoZGrid) { // 2+ possibilities

				v.z = gridZ2;
				AddRigidBodyToGrid(v, rigidbodies[i]); //112

				if (onTwoYGrid) { // 4 possibilities
					v.y = gridY2;
					AddRigidBodyToGrid(v, rigidbodies[i]); //122

					v.z = gridZ1;
					AddRigidBodyToGrid(v, rigidbodies[i]); //121
				}
			}
			else {
				if (onTwoYGrid) { // 2 possibilities
					v.y = gridY2;
					AddRigidBodyToGrid(v, rigidbodies[i]); //121
				}
			}
		}
	}
	//std::cout << "filledcells: \n";
	for (int i = 0; i < filledCells.size(); i++) {
		//std::cout << "\n";
		for (int j = 0; j < filledCells[i]->size(); j++) {
			for (int k = j + 1; k < filledCells[i]->size(); k++) {
				PotentialCollision pc = PotentialCollision();
				pc.rigidBodies[0] = filledCells[i]->at(j);
				pc.rigidBodies[1] = filledCells[i]->at(k);
				collision.push_back(pc);
				//std::cout << "collision  " << pc.rigidBodies[0]->position << " -- " << pc.rigidBodies[1]->position<< " \n";
			}
		}
	}

	//Fix to get only 1 time the contact
	for (int i = collision.size()-1; i >-1; i--) {
		for (int j = i-1; j > -1; j--) {
			if (collision.at(i).rigidBodies[0] == collision.at(j).rigidBodies[0] && collision.at(i).rigidBodies[1] == collision.at(j).rigidBodies[1]) {
				collision.erase(collision.begin() + i);
				break;
			}
		}
	}

	/*std::cout << "Potential collision: \n";
	for(int i = 0; i < collision.size(); i++) {
		std::cout << collision.at(i).rigidBodies[0]->position << " " << collision.at(i).rigidBodies[1]->position << std::endl;
	}
	std::cout << "=========================== \n";*/

	return collision;
}

void Grid::AddRigidBodyToGrid(Vector3D v, RigidBody* rb)
{
	//std::cout << "Add rb to "<< v <<"\n";
	auto iterator = rigidbodiesInCell.find(v);
	if (iterator == rigidbodiesInCell.end()) {
		std::vector<RigidBody*> newVector = {};
		newVector.push_back(rb);
		rigidbodiesInCell[v] = newVector;
		filledCells.push_back(&rigidbodiesInCell[v]);
	}
	else {

		iterator->second.push_back(rb);
	}
}
