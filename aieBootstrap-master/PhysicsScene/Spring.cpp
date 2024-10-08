#include "Spring.h"
#include "Rigidbody.h"

Spring::Spring(Rigidbody* body1, Rigidbody* body2,
	float springCoefficient, float damping, float restLength,
	glm::vec2 contact1, glm::vec2 contact2) : PhysicsObject(JOINT){
	m_body1 = body1;
	m_body2 = body2;

	m_springCoefficient = springCoefficient;
	m_damping = damping;
	m_restLength = restLength;

	m_contact1 = getContact1();
	m_contact2 = getContact2();
}
Spring::Spring(Rigidbody* body1, Rigidbody* body2, float damping, float springCoefficient) : PhysicsObject(JOINT) {
	m_body1 = body1;
	m_body2 = body2;

	m_springCoefficient = springCoefficient;
	m_damping = damping;
	m_restLength = glm::distance(body1->getPosition(), body2->getPosition());

	m_contact1 = getContact1();
	m_contact2 = getContact2();
}


void Spring::fixedUpdate(glm::vec2 gravity, float timeStep)
{
	// get the world coordinates of the ends of the springs
	glm::vec2 p1 = getContact1();
	glm::vec2 p2 = getContact2();
	float length = glm::distance(p1, p2);
	// Gets direction by normalizing contact point 1 and 2
	glm::vec2 direction = glm::normalize(p2 - p1);

	// apply damping
	glm::vec2 relativeVelocity = m_body2->getVelocity() - m_body1->getVelocity();
	// F = -kX - bv
	glm::vec2 force = direction * m_springCoefficient * (m_restLength - length) -
		m_damping * relativeVelocity;
	m_body1->applyForce(-force * timeStep, p1 - m_body1->getPosition());
	m_body2->applyForce(force * timeStep, p2 - m_body2->getPosition());
}

glm::vec2 Spring::getContact1() {
	return m_body1 ? m_body1->toWorld(m_contact1) : m_contact1;
}

glm::vec2 Spring::getContact2() {
	return m_body1 ? m_body2->toWorld(m_contact1) : m_contact1;
}

void Spring::draw() {
	aie::Gizmos::add2DLine(getContact1(), getContact2(), glm::vec4(1, 1, 1, 1));
}