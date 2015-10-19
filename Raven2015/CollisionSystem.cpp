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

			std::unique_ptr<sf::Vector2f> collisionPoint;
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

	cout << "Collision occurred" << endl;

	ex::ComponentHandle<Rigidbody> leftRigidbody = event.leftRigidbody;
	ex::ComponentHandle<Rigidbody> rightRigidbody = event.rightRigidbody;
	sf::Vector2f avgVelocity = (leftRigidbody->velocity + rightRigidbody->velocity) / 2.0f;
	
	if (event.leftBoxCollider->collisionSettings.find(COLLISION_LAYER_SETTINGS_FIXED) != event.leftBoxCollider->collisionSettings.end()) {
		rightRigidbody->velocity -= avgVelocity;
	} 

	if (event.rightBoxCollider->collisionSettings.find(COLLISION_LAYER_SETTINGS_FIXED) != event.rightBoxCollider->collisionSettings.end()) {
		leftRigidbody->velocity -= avgVelocity;
	}
}

/*
* Tests whether two entities' colliders register a collision.
*/
std::unique_ptr<sf::Vector2f> CollisionSystem::testCollision(ex::Entity leftEntity,
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

	// Return whether the distance between objects is less than their reach towards each other on BOTH axes
	if (xDiff <= xReach && yDiff <= yReach) {

		if (leftBoxCollider->collisionSettings.find(COLLISION_LAYER_SETTINGS_SOLID) != leftBoxCollider->collisionSettings.end() &&
			rightBoxCollider->collisionSettings.find(COLLISION_LAYER_SETTINGS_SOLID) != rightBoxCollider->collisionSettings.end()) {

			if (!(leftBoxCollider->collisionSettings.find(COLLISION_LAYER_SETTINGS_FIXED) != leftBoxCollider->collisionSettings.end() &&
				rightBoxCollider->collisionSettings.find(COLLISION_LAYER_SETTINGS_FIXED) != rightBoxCollider->collisionSettings.end())) {
			
					// Approximate the collision point as directly in between the objects
					std::unique_ptr<sf::Vector2f> collisionPoint(new sf::Vector2f(0.5f * xDiff, 0.5f * yDiff));
					collisionPoint->x += std::min(leftColliderX, rightColliderX);
					collisionPoint->y += std::min(leftColliderY, rightColliderY);

					// Notify all of those listening for collisions that a collision has occurred
					return collisionPoint;
			}
		}
    }
    return nullptr;
}