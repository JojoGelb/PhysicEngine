#include "NaiveRigidBodyContactGenerator.h"

unsigned int NaiveRigidBodyContactGenerator::AddContact(std::vector<RigidBodyContactTest*>& contacts, unsigned int limit) const
{
	int i, j;

	int numAddedContacts = 0;
	//avoid double contacts using this nested loop
	for (i = 0; i < rigidbodies->size(); i++) {
		RigidBody* p = rigidbodies->at(i);
		for (j = i + 1; j < rigidbodies->size(); j++) {
			//for (j = 0; j < particles->size(); j++) {
			RigidBody* p2 = rigidbodies->at(j);
			if (p == p2) {
				continue; //shouldn't happen now
			}

			float distance = p->position.DistanceTo(p2->position);

			if (distance < 2 * radius) {

				RigidBodyContactTest* contact = new RigidBodyContactTest(p, p2, 1);

				contact->contactNormal = (contact->rigidbody[0]->position - contact->rigidbody[1]->position).Normalize();

				float maxDist = radius * 2;

				contact->penetration = maxDist - contact->rigidbody[0]->position.DistanceTo(contact->rigidbody[1]->position);
				contacts.push_back(contact);
				numAddedContacts++;

				if (contacts.size() >= limit) {
					return numAddedContacts;
				}
			}
		}
	}

	return numAddedContacts;
}
