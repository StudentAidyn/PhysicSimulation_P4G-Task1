#include "PhysicsScene.h"

// Physics Scene contstructor, sets the timestep to 0.01f and the Gravity to (0,0)
PhysicsScene::PhysicsScene() {
	m_timeStep = 0.01f;
	m_gravity = glm::vec2(0, 0);
}

// Destructor, should destroy the actors in m_actors (vector list)
PhysicsScene::~PhysicsScene() {
	for (auto & physObjects : m_actors) {
		delete physObjects;
	}
}

// Adds an Physics Object -> actor to the back of m(ember)_actors vector list
void PhysicsScene::addActor(PhysicsObject *actor) {
	m_actors.push_back(actor);
}


void PhysicsScene::removeActor(PhysicsObject* actor) {
	//m_actors.erase()
}