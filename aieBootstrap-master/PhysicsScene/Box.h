#pragma once
#include "Rigidbody.h"

/// Box is used when performing phyics on 2D even sided and 90 Degree angled 4 cornered objects. 
/// 
/// Box object derives from the Rigidbody class


class Box : public Rigidbody {
public:
	Box() {}
	/// Basic constructor
	///
	/// The basic constructor takes the absolute basics of Box Physics Objects including position, size, mass and colour.
	/// The rest of the values are pre-filled and passed into the Rigidbody class.
	/// @param x is the starting x coordinate of the object
	/// @param y is the starting y coordinate of the object
	/// @param width is the width of the box
	/// @param height is the height of the box
	/// @param mass is the total matter of the object
	/// @param colour is the colour of the Box
	Box(float x, float y, float width, float height, float mass, glm::vec4 colour);


	/// double float constructor
	///
	/// The double float constructor takes all values, the position and size values are passed through as 2 sets of floats.
	/// @param x is the starting x coordinate of the object
	/// @param y is the starting y coordinate of the object
	/// @param width is the width of the box
	/// @param height is the height of the box
	/// @param orientation is the starting orientation of the object
	/// @param velocity is the starting Linear Velocity of the object
	/// @param angularVelocity is the starting Angular Velocity of the object
	/// @param mass is the total matter of the object
	/// @param elasticity is the amount of elasticity of the object
	/// @param colour is the colour of the Box
	Box(float x, float y, float width, float height, float orientation, glm::vec2 velocity, float angularVelocity, float mass, float elasticity, glm::vec4 colour);

	/// double Vec2 constructor
	///
	/// The double Vec2 constructor takes all values, the position and size values are passed through as glm::vec2 s.
	/// @param position is the starting x and y coordinates of the object
	/// @param extents is the half width and height of the box
	/// @param orientation is the starting orientation of the object
	/// @param velocity is the starting Linear Velocity of the object
	/// @param angularVelocity is the starting Angular Velocity of the object
	/// @param mass is the total matter of the object
	/// @param elasticity is the amount of elasticity of the object
	/// @param colour is the colour of the Box
	Box(glm::vec2 position, glm::vec2 extents, float orientation, glm::vec2 velocity, float angularVelocity, float mass, float elasticity, glm::vec4 colour);
	
	/// Box Destructor [-Empty-]
	~Box();

	/// Overriden draw function
	///
	/// Adds 2 triangles to the Gizmo draw function to create a box.
	void draw() override;

	/// Extents Getter Function
	///
	/// @returns glm::vec2 m_extents 
	glm::vec2 getExtents() const { return m_extents; }

	/// Width Getter Funciton
	///	
	/// Since extents is half lengths, to return the width of the box, the x value is multiplied by 2.
	/// @returns float m_extents.x * 2
	float getWidth() const { return (m_extents.x * 2); }

	/// Height Getter Function
	/// 
	/// Since extents is half lengths, to return the height of the box, the y value is multiplied by 2.
	/// @returns float m_extents.y * 2
	float getHeight() const { return (m_extents.y * 2); }

	/// Colour Setter
	///
	/// @param colour is the new colour the box is being set to
	void setColour(glm::vec4 colour) { m_colour = colour; }

	/// Checks the collisions between 2 boxes corners
	///
	/// This function checks for collisions between 2 box objects by using the Separating Axis Theorem. 
	/// https://www.youtube.com/watch?v=Nm1Cgmbg5SQ&t=26s&ab_channel=GamesWithGabe <- Used to understand the Theorem
	/// @param box is the other box that the collisions are being checked against
	/// @param contacts is the number of contacts devided by the total contacts
	/// @param numContacts is the number of contacts parrallel 
	/// @param pen is the depth of penetration
	/// @param edgeNormal is the normal of the 2 boxes
	bool checkBoxCorners(Box& box, glm::vec2& contact, int& numContacts, float& pen, glm::vec2& edgeNormal);


protected:
	/// Extents is the halfedge lengths of Box Objects
	glm::vec2 m_extents;

	/// Colour of the box object
	glm::vec4 m_colour; 
};

