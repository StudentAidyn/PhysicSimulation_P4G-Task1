#include "Rigidbody.h"
#include "PhysicsScene.h"



Rigidbody::Rigidbody(ShapeType shapeID, glm::vec2 position, float orientation, glm::vec2 velocity, float angularVelocity, float mass, float elasticity, float moment) :
	PhysicsObject(shapeID), m_position(position), m_velocity(velocity), m_angularVelocity(angularVelocity), m_orientation(orientation), m_mass(mass) {
	m_elasticity = elasticity;
	m_moment = moment;
	m_isKinematic = false;

	m_localX = glm::vec2(1);
	m_localY = glm::vec2(1);
}

Rigidbody::~Rigidbody() {

}


// it is currently okay to apply gravity after calculating the new position of the Rigidbody object -------------------+
void Rigidbody::fixedUpdate(glm::vec2 gravity, float timeStep) {

	float cs = cosf(m_orientation);
	float sn = sinf(m_orientation);
	m_localX = glm::normalize(glm::vec2(cs, sn));
	m_localY = glm::normalize(glm::vec2(-sn, cs));


	if (m_isKinematic) {
		m_velocity = glm::vec2(0);
		m_angularVelocity = 0;
		return;
	}

	if (m_lockRotation) { m_angularVelocity = 0; }
	if (isPositionLocked()) { m_velocity = glm::vec2(0); }
	else { m_velocity = glm::vec2(m_lockX ? 0 : m_velocity.x, m_lockY ? 0 : m_velocity.y); }

	

	if (m_hasLinearDrag) { m_velocity -= m_velocity * m_linearDrag * timeStep; }
	if (m_hasAngularDrag) { m_angularVelocity -= m_angularVelocity * m_angularDrag * timeStep; }

	if (length(m_velocity) < MIN_LINEAR_THRESHOLD) {
		m_velocity = glm::vec2(0, 0);
	}

	if (abs(m_angularVelocity) < MIN_ANGULAR_THRESHOLD) {
		m_angularVelocity = 0;
	}

	m_position += m_velocity * timeStep;
	applyForce(gravity * m_mass * timeStep, glm::vec2(0)); // currently passing through empty v2, stops the spin :) force is central :D

	m_orientation += m_angularVelocity * timeStep;



}

// adds force to the rigidbody using f = m*a altered to a = f/m
void Rigidbody::applyForce(glm::vec2 force, glm::vec2 pos) {
	if (!isPositionLocked()) { m_velocity += glm::vec2(m_lockX ? 0 : force.x, m_lockY ? 0 : force.y) / m_mass; }
	if (!m_lockRotation) { m_angularVelocity += (force.y * pos.x - force.x * pos.y) / getMoment(); }
}

void Rigidbody::resolveCollision(Rigidbody* actor2, glm::vec2 contact, glm::vec2* collisionNormal, float pen){


	// find the vector between their centres, or use the provided direction
	// of force, and make sure it's normalised
	glm::vec2 normal = glm::normalize(collisionNormal ? *collisionNormal :
		actor2->m_position - m_position);
	// get the vector perpendicular to the collision normal
	glm::vec2 perp(normal.y, -normal.x);
	// determine the total velocity of the contact points for the two objects,
	// for both linear and rotational
	// 'r' is the radius from axis to application of force
	float r1 = glm::dot(contact - m_position, -perp);
	float r2 = glm::dot(contact - actor2->m_position, perp);
	// velocity of the contact point on this object
	float v1 = glm::dot(m_velocity, normal) - r1 * m_angularVelocity;
	// velocity of contact point on actor2
	float v2 = glm::dot(actor2->m_velocity, normal) + r2 * actor2->m_angularVelocity;

	float elasticity = (getElasticity() + actor2->getElasticity()) / 2.0f;

	/* Kinetic Energy Prior to applying force to objects */
	//float kePre = getKineticEnergy() + actor2->getKineticEnergy();

	if (v1 > v2) // they're moving closer
	{
		// calculate the effective mass at contact point for each object
		// ie how much the contact point will move due to the force applied.
		float mass1 = 1.0f / (1.0f / getMass() + (r1 * r1) / getMoment());
		float mass2 = 1.0f / (1.0f / actor2->getMass() + (r2 * r2) / actor2->getMoment());
		
		glm::vec2 force = (1.0f + elasticity) * mass1 * mass2 /
			(mass1 + mass2) * (v1 - v2) * normal;
		//apply equal and opposite forces
		applyForce(-force, contact - m_position);
		actor2->applyForce(force, contact - actor2->m_position);

		if(collisionCallback != nullptr)
			collisionCallback(actor2);
		if (actor2->collisionCallback)
			actor2->collisionCallback(this);
		
		/* Kinetic Energy After applying forces to objects */
		//float kePost = getKineticEnergy() + actor2->getKineticEnergy();

		/* Comparing the PRIOR and AFTER Kinetic Energy, Stating when loss of kinetic energy */
		/*float deltaKE = kePost - kePre;
		if (deltaKE > kePost * 0.01f) { std::cout << "Kinetic Energy discrepancy greater than 1% detected!! " << kePre << " -> " << kePost; } */

		if (pen > 0) { PhysicsScene::ApplyContactForces(this, actor2, normal, pen); }
	}


}

float Rigidbody::getPotentialEnergy(){
	float potentialEnergy = -getMass() * glm::dot(PhysicsScene::getGravity(), getPosition());

	return potentialEnergy;
}


float Rigidbody::getKineticEnergy() {
	float kineticEnergy = 0.5f * (getMass() * glm::dot(m_velocity, m_velocity) + getMoment() * (m_angularVelocity * m_angularVelocity));

	return kineticEnergy;
}
