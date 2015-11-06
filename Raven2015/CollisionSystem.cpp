#include "CollisionSystem.h"
#include "ComponentLibrary.h"
#include "entityx\Entity.h"
#include <algorithm>            //For std::min

using namespace Raven;

/*
* Iterate through all objects with Colliders and emit CollisionEvents.
*/
void CollisionSystem::update(ex::EntityManager &es, ex::EventManager &events,
    ex::TimeDelta dt) {

    collisionMap.clear();

    es.each<BoxCollider>([&](ex::Entity leftEntity, BoxCollider &leftCollider) {
        es.each<BoxCollider>([&](ex::Entity rightEntity, BoxCollider &rightCollider) {

            std::shared_ptr<sf::Vector2f> collisionPoint;
            // If we are checking for a collision between two different objects...
            if (leftEntity != rightEntity && (collisionPoint = testCollision(leftEntity, rightEntity))) {

                // Check whether we already have a record of collisions involving these two entities
                std::map<ex::Entity, std::set<ex::Entity>>::iterator it1 = collisionMap.find(leftEntity);
                std::map<ex::Entity, std::set<ex::Entity>>::iterator it2 = collisionMap.find(rightEntity);

                // If leftEntity has collided with rightEntity one-way, do nothing
                if (collisionMap.find(leftEntity) != collisionMap.cend() &&
                    collisionMap[leftEntity].find(rightEntity) != collisionMap[leftEntity].cend()) {

                    return;
                }

                // If rightEntity has collided with leftEntity one-way, do nothing
                if (collisionMap.find(rightEntity) != collisionMap.cend() &&
                    collisionMap[rightEntity].find(leftEntity) != collisionMap[rightEntity].cend()) {

                    return;
                }

                collisionMap[leftEntity].insert(rightEntity);
                events.emit<CollisionEvent>(leftEntity, rightEntity, *collisionPoint, &events);
            }
        });
    });
}

void CollisionSystem::receive(const CollisionEvent &event) {

    //cout << "Collision occurred" << endl;
	ex::ComponentHandle<Rigidbody> leftRigidbody = event.leftRigidbody;
	ex::ComponentHandle<Rigidbody> rightRigidbody = event.rightRigidbody;
    sf::Vector2f leftVelocity = event.leftRigidbody->velocity;
	sf::Vector2f rightVelocity = event.rightRigidbody->velocity;
	sf::Vector2f relVel = rightVelocity - leftVelocity;
	double dotProduct = (relVel.x * event.collisionPoint.x) + (relVel.y * event.collisionPoint.y);

	if (dotProduct <= 0) {
		double j = -(1 - E) * dotProduct;
		j /= (2 / STANDARD_MASS);

		sf::Vector2f newVel((event.collisionPoint.x * j), (event.collisionPoint.y * j));
		leftRigidbody->velocity -= 1 / STANDARD_MASS * newVel;
		rightRigidbody->velocity += 1 / STANDARD_MASS * newVel;
	}
}

/*
* Tests whether two entities' colliders register a collision.
*/
std::shared_ptr<sf::Vector2f> CollisionSystem::testCollision(ex::Entity leftEntity,
    ex::Entity rightEntity) {

    // Grab relevant components from left Entity
    ex::ComponentHandle<Transform> leftTransform = leftEntity.component<Transform>();
    ex::ComponentHandle<BoxCollider> leftBoxCollider = leftEntity.component<BoxCollider>();
    /*ex::ComponentHandle<CircleCollider> leftCircleCollider =
    leftEntity.component<CircleCollider>();*/

    // Grab relevant components from right Entity
    ex::ComponentHandle<Transform> rightTransform = rightEntity.component<Transform>();
    ex::ComponentHandle<BoxCollider> rightBoxCollider = rightEntity.component<BoxCollider>();
    /*ex::ComponentHandle<CircleCollider> rightCircleCollider =
    leftEntity.component<CircleCollider>();*/

    // Determine which types of Colliders we are dealing with
    if (false) {
        // TODO
    }

	// If both objects are fixed, do not need to do send collision event
	if (!(leftBoxCollider->collisionSettings.find(COLLISION_LAYER_SETTINGS_FIXED) != leftBoxCollider->collisionSettings.end() &&
		rightBoxCollider->collisionSettings.find(COLLISION_LAYER_SETTINGS_FIXED) != rightBoxCollider->collisionSettings.end())) {

		// Check that both objects are solid, otherwise collision event doesn't need to occur
		if (leftBoxCollider->collisionSettings.find(COLLISION_LAYER_SETTINGS_SOLID) != leftBoxCollider->collisionSettings.end() &&
			rightBoxCollider->collisionSettings.find(COLLISION_LAYER_SETTINGS_SOLID) != rightBoxCollider->collisionSettings.end()) {

			// Check if both objects are on same layer.  Collision event only needs to occur for objects on same layer
			for (std::string layer : leftBoxCollider->layers) {
				if (rightBoxCollider->layers.find(layer) != rightBoxCollider->layers.end()) {

					//Calculate the exact location of each collider
					float leftColliderX = leftTransform.get()->transform.x + leftBoxCollider.get()->originOffset.x;
					float leftColliderY = leftTransform.get()->transform.y + leftBoxCollider.get()->originOffset.y;
					float rightColliderX = rightTransform.get()->transform.x + rightBoxCollider.get()->originOffset.x;
					float rightColliderY = rightTransform.get()->transform.y + rightBoxCollider.get()->originOffset.y;

					// Calculate the dimensions of a rectangle with the colliders at the corners
					float xDiff = abs(leftColliderX - rightColliderX);
					float yDiff = abs(leftColliderY - rightColliderY);

					//Acquire one-half the width/height "reach" from each collider's origin location
					float xReach = (leftBoxCollider.get()->width + rightBoxCollider.get()->width) * 0.5f;  // x-axis left reach
					float yReach = (leftBoxCollider.get()->height + rightBoxCollider.get()->height) * 0.5f;  // y-axis left reach

					std::shared_ptr<sf::Vector2f> collisionNormal(new sf::Vector2f(0.0f, 0.0f));
					bool collision = false;

					if (xDiff <= xReach) {
						if (xDiff <= yDiff) {
							collisionNormal->x = 1.0f;
							collision = true;
						}
					}

					if (yDiff <= yReach) {
						if (yDiff <= xDiff) {
							collisionNormal->y = 1.0f;
							collision = true;
						}
					}

					if (collision) {
						return collisionNormal;
					}
				}
			}

        }
    }
    return nullptr;
}