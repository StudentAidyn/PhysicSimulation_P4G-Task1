#pragma once
#include "Rigidbody.h"
class Sphere : public Rigidbody{
public:
	/* Sphere takes firstly Object related information : Position, Radius, Orientation,
	Then it takes Physics related info : Velocity, Angular Velocity, Mass, Elasticity,
	Finally it takes a colour :D */
	/// Sphere Constructor
	/// 
	/// The first parameters are object information
	/// @param x is the starting X position
	/// @param y is the starting Y position
	/// @param radius is the radius of the circle/sphere
	/// @param orientation is the starting rotation
	/// 
	/// Then Physics information
	/// @param velocity is the starting velocity of the sphere
	/// @param angularVelocity is the starting angular velocity of the sphere
	/// @param mass is the total matter within the object
	/// @param elasticity is the total energy conservation when colliding with other objects
	/// 
	/// finally the last Parameter is passed through
	/// @param colour is the colour of the Sphere
	Sphere(float x, float y, float radius, float orientation, glm::vec2 velocity, float angularVelocity, float mass, float elasticity, glm::vec4 colour);
	/// Sphere Constructor
	/// 
	/// The first parameters are object information
	/// @param position is the starting X and Y position
	/// @param radius is the radius of the circle/sphere
	/// @param orientation is the starting rotation
	/// 
	/// Then Physics information
	/// @param velocity is the starting velocity of the sphere
	/// @param angularVelocity is the starting angular velocity of the sphere
	/// @param mass is the total matter within the object
	/// @param elasticity is the total energy conservation when colliding with other objects
	/// 
	/// finally the last Parameter is passed through
	/// @param colour is the colour of the Sphere
	Sphere(glm::vec2 position, float radius, float orientation, glm::vec2 velocity, float angularVelocity, float mass, float elasticty, glm::vec4 colour);
	/// Basic Style Constructor
	/// 
	/// This is the basic Constructor
	/// @param position is the starting X and Y position
	/// @param velocity is the starting velocity of the object
	/// @param radius is the radius of the circle
	/// @param mass is the total matter of the object
	/// @param colour is the colour of the Sphere
	Sphere(glm::vec2 position, glm::vec2 velocity, float radius, float mass, glm::vec4 colour);
	~Sphere();

	/// Adds Circle to Gizmo.
	virtual void draw();

	float getRadius() { return m_radius; }
	glm::vec4 getColour() { return m_colour; }

protected:
	/// Radius of the Sphere
	float m_radius;
	/// Colour of the Sphere
	glm::vec4 m_colour;
};

