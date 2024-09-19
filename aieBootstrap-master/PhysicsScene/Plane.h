#pragma once
#include "PhysicsObject.h"
#include "Application.h"
#include "Gizmos.h"
#include "Rigidbody.h"
#include <functional>

/// Plane Object is derived from PhysicsObject directly as this object is not effected by External Physics
///
/// Plane Object is unaffected by Gravity or Rigidbodys collisions, the object itself is treated as a static Object, static meaning it can't be changed.
/// Plane static objects have 2 defining features, the object is treated as if it has infinite mass, Planes stretch collision below and along its length infinitely. 


class Plane : public PhysicsObject
{
public:
	/// Plane Default Constructor
	Plane();
	/// Plane Paramatized Constructor
	/// 
	/// Sets the Plane object and calls the PhysicsObject constructor passing PLANE into the argument.
	/// @param normal is the current Normal / Orientation of the Plane
	/// @param distance is the distance from the world origin
	/// @param elasticity is the elasticity of the Plane Object
	Plane(glm::vec2 normal, float distance, float elasticity);

	/// Plane Destructor [-empty-]
	~Plane();

	/// Calls Fixed Update - EMPTY
	void fixedUpdate(glm::vec2 gravity, float timeStep) override;
	/// Adds Plane Object with correct orientation and sizing to Gizmo
	void draw() override;
	/// Resets Distance to Origin to 0
	void resetPosition() override { m_distanceToOrigin = 0; }

	/// Applies force back on contact object
	/// 
	/// Since planes are Kinematic objects their mass is considered infinite. 
	/// Instead of using an infinite value to replicate this during the force formula, 
	/// the Plane's mass is removed, so total force can be correctly applied.
	/// The elasticity is calculated by summing the Collision Object's elasticity and the Plane's elasticity,
	/// elasticity is applied within the force formula. 
	void resolveCollision(Rigidbody* actor2, glm::vec2 contact);


	std::function<void(PhysicsObject*)> collisionCallback;


	/// Normal Getter function
	/// 
	/// @returns glm::vec2 m_normal
	glm::vec2 getNormal() { return m_normal; }
	/// Distance Getter function
	/// 
	/// @returns float m_distanceToOrigin
	float getDistance() { return m_distanceToOrigin; }

protected:
	/// The orientation of the object, declaring its normal
	glm::vec2 m_normal;
	/// Distance from position(0, 0) within the main scene
	float m_distanceToOrigin;
	/// Colour of the Plane Object
	glm::vec4 m_colour = glm::vec4(1, 1, 1, 1);
};

