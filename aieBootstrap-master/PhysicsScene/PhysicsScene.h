#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include "PhysicsObject.h"
// Objects
#include "Spring.h"
#include "Plane.h"
#include "Sphere.h"
#include "Box.h"

/// PhysicsScene controls PhysicsObject interactions.
/// 
/// PhysicsScene contains all physicsObjects that are being drawn into the main scene, it adds all phyisics objects to a Vector list and controls the physics objects interactions with one another.

class PhysicsScene {
public:
	/// Constructor
	/// 
	/// Sets timeStep and Gravity variables to default values
	PhysicsScene();
	/// Destructor
	/// 
	/// Clears all Physics Objects within its Pointer Vector List
	~PhysicsScene();

	/// Add's PhysicsObject to List
	/// 
	/// Using PushBack it adds a new PhysicsObject to the end of the Pointer Vector List
	/// @param actor is a PhysicsObject that interacts within the scene
	void addActor(PhysicsObject* actor);
	/// Removes PhysicsObject from List
	/// 
	/// Using the Parameter, the vector list is searched for an identical PhysicsObject element and the location is used to erase the object from the list.
	/// @param actor the PhysicsObject that is being removed
	void removeActor(PhysicsObject* actor);

	/// Updates PhysicsObjects within the list
	/// 
	/// Calls Update for each PhysicsObject within m_actors and Calls checkForCollision() function.
	/// @param dt is the delta time / change in time between frames
	void update(float dt);

	/// draw calls the draw functions within each PhysicsObject within m_actors
	void draw();

	/// Gravity Setter
	/// 
	/// Sets m_gravity to the passed paramater.
	/// @param gravity the new gravity value
	void setGravity(glm::vec2 gravity) { m_gravity = gravity; }

	/// Gravity Getter
	/// 
	/// @returns glm::vec2 m_gravity
	static glm::vec2 getGravity() { return m_gravity; }

	/// Time Step Setter
	/// 
	/// Sets m_timeStep to the passed parameter.
	/// @param timeStep the new timeStep value
	void setTimeStep(const float timeStep) { m_timeStep = timeStep; }

	/// Time Step Getter
	/// 
	/// @returns float m_timeStep
	float getTimeStep() const { return m_timeStep; }

	void checkForRemoved();


	/// Apply Contact Forces to objects that are colliding
	/// 
	/// Applies a small push back force without changing the Centre of Mass and applies Friction to objects
	/// @param body1 is a Rigidbody Object
	/// @param body2 is another Rigidbody Object
	/// @param norm is the normalised vector between body1 and body2
	/// @param pen is the penetration depth of the 2 Rigidbody Objects
	static void ApplyContactForces(Rigidbody* body1, Rigidbody* body2, glm::vec2 norm, float pen = 0);

	/// @returns the total energy within the Physics Engine calling GetEnerg() on each PhysicsObject within m_actors
	float getTotalEnergy();

	/// Checks for collisions between current PhysicsObjects within m_actors
	/// 
	/// Using the ShapeID within Each PhysicsObject within m_actors the correct collision check is called passing in the PhysicsObjects pointers as arguments.
	void checkForCollision();

	/// Total Shapes that can be collided with
	const int SHAPE_COUNT = 3;


	// --------------- Collisions --------------------
	// Sphere 2 Other Object
	/// Sphere to Sphere Collision Check
	/// 
	/// checks the collisions between 2 sphere objects by comparing the 2 radii of the spheres to the distance between the 2 objects positions. 
	/// If the distance total was less then the total radii then the objects would have to be colliding, then the sphere object would call sphere's collision resolution. 
	static bool sphere2Sphere(PhysicsObject*, PhysicsObject*);
	/// Sphere 2 Box Collision Check - Calls box2Sphere()
	static bool sphere2Box(PhysicsObject*, PhysicsObject*);
	/// Sphere 2 Plane Collision Check 
	///
	/// Using the Radius of the Sphere and the distance with respect to the normal of the plane, collision is calculated based on this.
	/// If a collision occurs Plane::resolveCollision() is Called.
	static bool sphere2Plane(PhysicsObject*, PhysicsObject*);

	// Box 2 Other Object
	/// Box to Box Collision Check
	///
	/// To Check whether the boxes have collided Box::checkBoxCorners() is called on each box. 
	/// If Collision is detected then Rigidbody::resolveCollision() is called.
	static bool box2Box(PhysicsObject*, PhysicsObject*);
	/// Box to Sphere Collision Check
	/// 
	/// Using the Raduis of the Sphere and the corners of each box, collision is checked between the closest corner and the distance between the box and sphere.
	/// If Collision is detected then Rigidbody::resolveCollision() is called.
	static bool box2Sphere(PhysicsObject*, PhysicsObject*);
	/// Box 2 Plane Collision Check - plane2Box() is called swapping the paramaters
	static bool box2Plane(PhysicsObject*, PhysicsObject*);

	// Plane 2 Other Object
	///Plane to Plane Collision Check - returns false since Planes can't ever collide
	static bool plane2Plane(PhysicsObject*, PhysicsObject*);
	/// Plane to Sphere Collision Check - sphere2Plane() is called swapping the paramaters
	static bool plane2Sphere(PhysicsObject*, PhysicsObject*);
	/// Plane to Box Collision Check
	///
	/// Using the plane's normal and comparing the distance with respect to the box's current rotation from the box to the plane collision is detected.
	/// If collision occurs Plane::resolveCollision() is called.
	static bool plane2Box(PhysicsObject*, PhysicsObject*);
	
	std::vector<PhysicsObject*> getShapesOfType(ShapeType type);
	
protected:
	/// current force of gravity applied to all objects
	static glm::vec2 m_gravity;
	
	/// time between applying physics calculations, if adjusted to over the recommended 0.01, it can cause unpredictable outcomes.
	float m_timeStep;

	/// Pointer Vector List of all the physics objects within the main scene
	std::vector<PhysicsObject*> m_actors;
};

