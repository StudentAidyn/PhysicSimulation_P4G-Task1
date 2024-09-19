#include "Sphere.h"

Sphere::Sphere(float x, float y, float radius, float orientation, glm::vec2 velocity, float angularVelocity, float mass, float elasticity, glm::vec4 colour) :
Rigidbody(SPHERE, glm::vec2(x, y), orientation, velocity, angularVelocity, mass, elasticity, (0.5f * mass * (radius * radius))) {
	m_radius = radius;
	m_colour = colour;
}

Sphere::Sphere(glm::vec2 position, float radius, float orientation, glm::vec2 velocity, float angularVelocity, float mass, float elasticity, glm::vec4 colour) :
	Rigidbody(SPHERE, position, orientation, velocity, angularVelocity, mass, elasticity, (0.5f * mass * radius * radius)) {
	m_radius = radius;
	m_colour = colour;
}

Sphere::Sphere(glm::vec2 position, glm::vec2 velocity, float radius, float mass, glm::vec4 colour) :
	Rigidbody(SPHERE, position, 0, velocity, 0, mass, 1, (0.5f * mass * radius * radius)) {
	m_radius = radius;
	m_colour = colour;
}

Sphere::~Sphere() {

}


void Sphere::draw() {
	glm::vec2 end = glm::vec2(std::cos(m_orientation), std::sin(m_orientation)) * m_radius;

	aie::Gizmos::add2DCircle(m_position, m_radius, 12, m_colour);
	//aie::Gizmos::add2DLine(m_position, m_position + end, glm::vec4(1, 1, 1, 1));
}