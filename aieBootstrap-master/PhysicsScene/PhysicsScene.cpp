#include "PhysicsScene.h"

// Physics Scene contstructor, sets the timestep to 0.01f and the Gravity to (0,0)
PhysicsScene::PhysicsScene() {
	m_timeStep = 0.01;
	// setting gravity to empty
	m_gravity = glm::vec2(0, 0);
}

// Destructor, should destroy the actors in m_actors (vector list)
PhysicsScene::~PhysicsScene() {
	for (auto physObjects : m_actors) {
		delete physObjects;
		physObjects = nullptr;
	}
	m_actors.erase(m_actors.begin(), m_actors.end());

}

/* Sets the Gravity level to 0,0 */
glm::vec2 PhysicsScene::m_gravity = glm::vec2(0, 0.0f);
/* With Help  from Michael - 
https://www.tutorialspoint.com/how-to-initialize-private-static-members-in-cplusplus#:~:text=We%20can%20put%20static%20members,we%20can%20assign%20some%20value. */


// Collision Functions and Combinations pointer
typedef bool(*fn)(PhysicsObject*, PhysicsObject*);

static fn collisionFunctionArray[] = {
	PhysicsScene::plane2Plane,     PhysicsScene::plane2Sphere, PhysicsScene:: plane2Box,
	PhysicsScene::sphere2Plane, PhysicsScene::sphere2Sphere, PhysicsScene::sphere2Box,
	PhysicsScene::box2Plane, PhysicsScene::box2Sphere, PhysicsScene::box2Box
};

std::vector<PhysicsObject*> PhysicsScene::getShapesOfType(ShapeType type) {
	std::vector<PhysicsObject*> shapes;
	if (m_actors.empty()) return shapes;
	for (auto pActor : m_actors) {
		if (pActor->getShapeID() == type) shapes.push_back(pActor);
	}
	return shapes;
}

// Update function passing in Delta Time (dt)
void PhysicsScene::update(float dt) {
	// update physics at a fixed time step

	static float accumulatedTime = 0.0f;
	accumulatedTime += dt;

	while (accumulatedTime >= m_timeStep)
	{
		for (auto pActor : m_actors)
		{
			pActor->fixedUpdate(getGravity(), m_timeStep);
		}

		accumulatedTime -= m_timeStep;
	}

	checkForCollision();

	checkForRemoved();
	// Checks energy output
	// std::cout << "Total Energy: " << getTotalEnergy() << std::endl;
}



void PhysicsScene::checkForCollision() {
	// check for collisions (ideally you'd want to have some sort of 
// scene management in place)
	size_t actorCount = m_actors.size();

	// need to check for collisions against all objects except this one.
	for (int outer = 0; outer < actorCount - 1; outer++)
	{
		for (int inner = outer + 1; inner < actorCount; inner++)
		{
			PhysicsObject* object1 = m_actors[outer];
			PhysicsObject* object2 = m_actors[inner];
			int shapeId1 = object1->getShapeID();
			int shapeId2 = object2->getShapeID();

			if (shapeId1 < 0 || shapeId2 < 0) { continue; }

			// using function pointers
			int functionIdx = (shapeId1 * SHAPE_COUNT) + shapeId2;
			fn collisionFunctionPtr = collisionFunctionArray[functionIdx];
			if (collisionFunctionPtr != nullptr)
			{
				// did a collision occur?
				collisionFunctionPtr(object1, object2);
			}
		}
	}
}

void PhysicsScene::draw() {
	for (auto pActor : m_actors) {
		pActor->draw();
	}
}

void PhysicsScene::checkForRemoved() {
	std::vector<PhysicsObject*> toRemove;
	for (auto remove : m_actors) {
		if (remove->getMarkedForRemoval()) { toRemove.push_back(remove); }
	}

	for (auto removed : toRemove) {
		removeActor(removed);
	}

	toRemove.clear();
}


// Adds an Physics Object -> actor to the back of m(ember)_actors vector list
void PhysicsScene::addActor(PhysicsObject *actor) {
	m_actors.push_back(actor);
}

// removes specified Physics Object from Vector List
void PhysicsScene::removeActor(PhysicsObject* actor) {
	auto item = std::find(m_actors.begin(), m_actors.end(), actor);

	if (item != m_actors.end()) {
		m_actors.erase(item);
	}
	else {
		std::cout << "UNDEFINED VECTOR ACTOR" << std::endl;
	}
}


float PhysicsScene::getTotalEnergy() {
	float total = 0;
	for (auto it = m_actors.begin(); it != m_actors.end(); it++) {
		PhysicsObject* obj = *it;
		total += obj->getEnergy();
	}
	return total;
}

// o/-<|:
// Different Collisions ------------------------------------------------------------------------+


// PLANES:

// Plane colliding with Plane, this is not possible as they are static objects
bool PhysicsScene::plane2Plane(PhysicsObject* obj1, PhysicsObject* obj2)
{
	return false;
}

// Plane object colliding with Sphere object
bool PhysicsScene::plane2Sphere(PhysicsObject* obj1, PhysicsObject* obj2)
{
	return sphere2Plane(obj2, obj1);
}

// Plane object colliding with Box object
bool PhysicsScene::plane2Box(PhysicsObject* obj1, PhysicsObject* obj2)
{
	Plane* plane = dynamic_cast<Plane*>(obj1);
	Box* box = dynamic_cast<Box*>(obj2);
	//if we are successful then test for collision
	if (box != nullptr && plane != nullptr)
	{
		int numContacts = 0;
		glm::vec2 contact(0, 0);
		float contactV = 0;
		// get a representative point on the plane
		glm::vec2 planeOrigin = plane->getNormal() * plane->getDistance();
		// check all four corners to see if we've hit the plane
		for (float x = -box->getExtents().x; x < box->getWidth(); x += box->getWidth())
		{
			for (float y = -box->getExtents().y; y < box->getHeight(); y += box->getHeight())
			{
				// get the position of the corner in world space
				glm::vec2 p = box->getPosition() + x * box->getLocalX() + y * box->getLocalY();
				float distFromPlane = glm::dot(p - planeOrigin, plane->getNormal());
				// this is the total velocity of the point in world space
				glm::vec2 displacement = x * box->getLocalX() + y * box->getLocalY();
				glm::vec2 pointVelocity = box->getVelocity() + box->getAngularVelocity() * glm::vec2(-displacement.y, displacement.x);
				// and this is the component of the point velocity into the plane
				float velocityIntoPlane = glm::dot(pointVelocity, plane->getNormal());
				// and moving further in, we need to resolve the collision
				if (distFromPlane < 0 && velocityIntoPlane <= 0)
				{
					numContacts++;
					contact += p;
					contactV += velocityIntoPlane;
				
				}
			}
		}
		// we've had a hit - typically only two corners can contact
		if (numContacts > 0)
		{
			plane->resolveCollision(box, contact / (float)numContacts);
			return true;
		}
	}
	return false;
}



// SPHERES:

// Sphere object colliding with Plane object
bool PhysicsScene::sphere2Plane(PhysicsObject* obj1, PhysicsObject* obj2) {
	Sphere* sphere = dynamic_cast<Sphere*>(obj1);
	Plane* plane = dynamic_cast<Plane*>(obj2);
	//if we are successful then test for collision
	if (sphere != nullptr && plane != nullptr) {
		glm::vec2 collisionNormal = plane->getNormal();
		float sphereToPlane = glm::dot(sphere->getPosition(), plane->getNormal()) - plane->getDistance();

		float intersection = sphere->getRadius() - sphereToPlane;
		float velocityOutOfPlane = glm::dot(sphere->getVelocity(), plane->getNormal());
		if (intersection > 0 && velocityOutOfPlane < 0) {
			glm::vec2 contact = sphere->getPosition() + (collisionNormal * -sphere->getRadius());
			plane->resolveCollision(sphere, contact);
			return true;
		}
	}
	return false;
}

// Sphere object colliding with Sphere object
bool PhysicsScene::sphere2Sphere(PhysicsObject* obj1, PhysicsObject* obj2)
{
	// try to cast objects to sphere and sphere
	Sphere* sphere1 = dynamic_cast<Sphere*>(obj1);
	Sphere* sphere2 = dynamic_cast<Sphere*>(obj2);
	// if we are successful then test for collision
	if (sphere1 != nullptr && sphere2 != nullptr) {
		float dist = glm::distance(sphere1->getPosition(), sphere2->getPosition());
		float penetration = sphere1->getRadius() + sphere2->getRadius() - dist;
		if (penetration > 0)
		{
			sphere1->resolveCollision(sphere2, (sphere1->getPosition() + sphere2->getPosition()) * 0.5f, nullptr, penetration);
			return true;
		}
	}
	return false;
}

// Sphere object colliding with Box object
bool PhysicsScene::sphere2Box(PhysicsObject* obj1, PhysicsObject* obj2) {
	return box2Sphere(obj2, obj1);
}


// BOXES:

// Box object colliding with Plane object
bool PhysicsScene::box2Plane(PhysicsObject* obj1, PhysicsObject* obj2) {
	return plane2Box(obj2, obj1);
}

// Box object colliding with Sphere object
bool PhysicsScene::box2Sphere(PhysicsObject* obj1, PhysicsObject* obj2) {
	Box* box = dynamic_cast<Box*>(obj1);
	Sphere* sphere = dynamic_cast<Sphere*>(obj2);
	if (box != nullptr && sphere != nullptr)
	{
		// transform the circle into the box's coordinate space
		glm::vec2 circlePosWorld = sphere->getPosition() - box->getPosition();
		glm::vec2 circlePosBox = glm::vec2(glm::dot(circlePosWorld, box->getLocalX()), glm::dot(circlePosWorld, box->getLocalY()));
		// find the closest point to the circle centre on the box by clamping the coordinates in box - space to the box's extents
		glm::vec2 closestPointOnBoxBox = circlePosBox;
		glm::vec2 extents = box->getExtents();
		if (closestPointOnBoxBox.x < -extents.x) closestPointOnBoxBox.x = -extents.x;
		if (closestPointOnBoxBox.x > extents.x) closestPointOnBoxBox.x = extents.x;
		if (closestPointOnBoxBox.y < -extents.y) closestPointOnBoxBox.y = -extents.y;
		if (closestPointOnBoxBox.y > extents.y) closestPointOnBoxBox.y = extents.y;
		// and convert back into world coordinates
		glm::vec2 closestPointOnBoxWorld = box->getPosition() + closestPointOnBoxBox.x * box->getLocalX() + closestPointOnBoxBox.y * box->getLocalY();
		glm::vec2 circleToBox = sphere->getPosition() - closestPointOnBoxWorld;
		float penetration = sphere->getRadius() - glm::length(circleToBox);
		if (penetration > 0)
		{
			glm::vec2 direction = glm::normalize(circleToBox);
			glm::vec2 contact = closestPointOnBoxWorld;
			box->resolveCollision(sphere, contact, &direction, penetration);
			return true;
		}
	}
	return false;
}

// Box object colliding with Box object
bool PhysicsScene::box2Box(PhysicsObject* obj1, PhysicsObject* obj2) {
	Box* box1 = dynamic_cast<Box*>(obj1);
	Box* box2 = dynamic_cast<Box*>(obj2);
	if (box1 != nullptr && box2 != nullptr) {
		glm::vec2 boxPos = box2->getPosition() - box1->getPosition();
		glm::vec2 norm(0, 0);
		glm::vec2 contact(0, 0);
		float pen = 0;
		int numContacts = 0;
		box1->checkBoxCorners(*box2, contact, numContacts, pen, norm);
		if (box2->checkBoxCorners(*box1, contact, numContacts, pen, norm)) {
			norm = -norm;
		}
		if (pen > 0) {
			box1->resolveCollision(box2, contact / float(numContacts), &norm, pen);
		}
		return true;
	}
	return false;

}


void PhysicsScene::ApplyContactForces(Rigidbody* body1, Rigidbody* body2, glm::vec2 norm, float pen) {
	float body2Mass = body2 ? body2->getMass() : INT_MAX;
	float body1Factor = body2Mass / (body1->getMass() + body2Mass);

	body1->setPosition(body1->getPosition() - body1Factor * norm * pen);

	if (body2) { body2->setPosition(body2->getPosition() + (1 - body1Factor) * norm * pen); }
		
}

