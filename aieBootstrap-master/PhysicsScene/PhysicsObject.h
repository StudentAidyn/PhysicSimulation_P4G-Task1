#pragma once
#include <glm/glm.hpp>

class PhysicsObject
{
protected:
	PhysicsObject() {}

public:
	virtual void fixedUpdate(glm::vec2 gravity, float timeStep) = 0;
	virtual void draw() = 0;
	virtual void resetPosition() {};
};

