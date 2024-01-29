#include "PhysicsScene.h"
#include <iostream>

// Physics Scene contstructor, sets the timestep to 0.01f and the Gravity to (0,0)
PhysicsScene::PhysicsScene() : m_timeStep(0.01f), m_gravity(glm::vec2(0, 0)){

}

// Destructor, should destroy the actors in m_actors (vector list)
PhysicsScene::~PhysicsScene() {
	for (PhysicsObject* physObjects : m_actors) {
		delete physObjects;
		physObjects = nullptr;
	}
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
			pActor->fixedUpdate(m_gravity, m_timeStep);
		}

		accumulatedTime -= m_timeStep;
	}
}

void PhysicsScene::draw() {
	for (auto pActor : m_actors) {
		pActor->draw();
	}
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

