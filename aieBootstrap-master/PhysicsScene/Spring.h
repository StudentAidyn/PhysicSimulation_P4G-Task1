#pragma once
#include "PhysicsObject.h"
class Rigidbody; // forward declaration of Rigidbody Class
/// Spring acts as a connector that pulls objects towards one another without collision.
class Spring : public PhysicsObject {
public:
	/// Spring Constructor
	Spring(Rigidbody* body1, Rigidbody* body2,
		float springCoefficient, float damping, float restLength,
		glm::vec2 contact1 = glm::vec2(0, 0), glm::vec2 contact2 = glm::vec2(0, 0));
	/// Spring Constructor but rest length is set by the distance between body1 and body2
	Spring(Rigidbody* body1, Rigidbody* body2,
		float damping, float springCoefficient);
	/// Destructor -empty-
	~Spring() {}

	/// Fixed Update apply forces to body1 and body2 based on an adjusted Hooke's Law.
	/// Then the force of the spring is calculated, Hooke's Law will be used to calculate the Springs force:
	/// Force = -spring constant (spring coefficient) * x (the displacement of the spring)
	/// This won't work for the simulation as it will cause the spring to oscillate back and forth forever,
	/// to fix this a damping is used to simulate the reduction of mechanical energy.
	/// The new Formula is:
	/// Force = -spring constant (spring coefficient) * x (the displacement of the spring) - damping * relative velocity
	/// x is set by Normalizing the 2 contact points using getContact1() and getContact2().
	/// Relative Velocity is set by comparing body1 and body2's velocity.
	/// Sources:
	/// https://labs.phys.utk.edu/mbreinig/phys221core/modules/m3/Hooke's%20law.html#:~:text=The%20force%20exerted%20by%20a,directed%20towards%20its%20equilibrium%20position.&text=F%20%3D%20%2Dkx.,is%20called%20the%20spring%20constant.
	/// https://gafferongames.com/post/spring_physics/
	void fixedUpdate(glm::vec2 gravity, float timeStep);
	/// Draw adds a 2D Line to Gizmos using getContact1() and getContact2() as the start and end of the line.
	virtual void draw();

	/// Gets the world coordinate of the contact point if body1 isn't nullptr
	glm::vec2 getContact1();
	/// Gets the world coordinate of the contact point if body1 isn't nullptr
	glm::vec2 getContact2();


	// getter and setters
	void setBody1(Rigidbody* body) { m_body1 = body; }
	void setBody2(Rigidbody* body) { m_body2 = body; }

	float getDamping() { return m_damping; }
	float getRestLength() { return m_restLength; }
	float getCoefficient() { return m_springCoefficient; }
protected:
	/// A physicsObject spring is connected to
	Rigidbody* m_body1;
	/// A physicsObject spring is connected to
	Rigidbody* m_body2;

	/// the contact point on the physics object
	glm::vec2 m_contact1;
	/// the contact point on the physics object
	glm::vec2 m_contact2;

	/// damping is the mechanical energy reduction
	float m_damping;
	/// is the distance that the object doesn't apply any force at
	float m_restLength;
	/// the restoration force of the spring
	float m_springCoefficient; // the restoring force;

};

