#pragma once
#include <glm/glm.hpp>

class PhysicsScene;


/// ShapeType includes the possible available physic objects
/// 
/// Each Shape type is a value ranging from -1 to X number. 
enum ShapeType {
	/// JOINT spring like connector
	JOINT = -1,
	/// PLANE solid line object
	PLANE = 0,
	/// SPHERE solid circle object
	SPHERE,
	/// BOX solid square object
	BOX
};

/// PhysicsObject class is the base class to all physics objects used within the main 
class PhysicsObject
{
protected:
	PhysicsObject() : m_elasticity(1), m_shapeID(SPHERE) {}

	/// Physics Object Constructor
	/// 
	/// @param a_shapeID is the shape of the Physics Object and used to define collision types within the PhysicsScene Class.
	PhysicsObject(ShapeType a_shapeID) : m_shapeID(a_shapeID) {}

public:
	~PhysicsObject() {}
	/// Fixed Update is a Pure Virtual function
	/// 
	/// @param gravity is the current gravity being used in the scene
	/// @param timeStep is the current time between physics updates
	virtual void fixedUpdate(glm::vec2 gravity, float timeStep) = 0;

	/// Draw is a Pure Virtual function
	/// 
	/// Draw is used to draw objects within the main scene within the inheriting classes.
	virtual void draw() = 0;

	/// Virtual Energy Getter function
	/// 
	/// @returns float 0 by default
	virtual float getEnergy() { return 0; }

	/// Virtual Reset Position function
	/// 
	/// Function is empty by default
	virtual void resetPosition() {};

	void setForRemoval() { m_markedForRemoval = true;  }
	bool getMarkedForRemoval() const { return m_markedForRemoval; }
	/// Shape ID Getter function
	/// 
	/// @returns m_shapeID of the Physics Object
	ShapeType getShapeID() const { return m_shapeID; }

	/// Elasticity Getter function
	/// 
	/// @returns m_elasticity, the elasticity of a Physics Object
	float getElasticity() const { return m_elasticity; }

protected:
	/// The shapes identifiable type using an enum
	ShapeType m_shapeID;
	/// The physics object's elasticity 
	float m_elasticity;

	/// Checks if the object is being removed
	bool m_markedForRemoval = false;
};

