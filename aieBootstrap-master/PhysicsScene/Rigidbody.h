#pragma once
#include "PhysicsObject.h"
#include <glm/glm.hpp>
#include <iostream>
#include "Renderer2D.h"
#include "Gizmos.h"
#include <functional>

/// Rigidbody derives from PhysicsObject() and is the Base Class to Physics Effected Game Objects.

class Rigidbody : public PhysicsObject {
public:
    Rigidbody() {}
    /// Constructor 
    /// Sets parameter values and passes ShapeType through to PhysicsObject.
    /// @param shapeID is the type of shape being passed through, for rigidbodies this is 1 of 2, BOX or SPHERE
    /// @param position is the starting position of the Rigidbody object
    /// @param orientation is the starting angle of the object
    /// @param velocity is the initial linear velocity of the object
    /// @param angularVelocity is the initial rotational velocity of the object
    /// @param mass is total matter of the object
    /// @param elasticity is the total energy conservation of the object
    /// @param moment is the moment of inertia, passed through as a direct answer to the formula for that shape.
    Rigidbody(ShapeType shapeID, glm::vec2 position, float orientation , //Object Specifics
        glm::vec2 velocity,  float angularVelocity, float mass, float elasticity, float moment); //Physics

    ~Rigidbody();

    /// Fixed Update
    /// Updates the rotation and variables relating to the current velocity and forces, this includes gravity and any other force.
    /// Update also checks if an object is Kinematic in which it applies and converts all velocity and forces into null/0 returning before updating the rest. 
    /// @param gravity is the current gravity within the PhysicsScene
    /// @param timeStep is teh current timeStep within the PhysicsScene
    virtual void fixedUpdate(glm::vec2 gravity, float timeStep);
    /// Energy Getter function
    /// getEnergy is an overriden function from PhysicsObject
    /// Get Energy gets the total energy from the Potential energy and Kinematic energy.
    /// https://en.cppreference.com/w/cpp/language/virtual
    float getEnergy() override { return getPotentialEnergy() + getKineticEnergy(); }
    /// Adds force to the velocity of the object depending on the total mass. 
    /// @param force is the force being applied to the Rigidbody object
    /// @param pos is the position of the force being applied, this mainly affects angular velocity
    void applyForce(glm::vec2 force, glm::vec2 pos);

    /// Collisions Resolution between 2 Rigidbodies
    /// 
    void resolveCollision(Rigidbody* actor2, glm::vec2 contact, glm::vec2* collisionNormal=nullptr, float pen = 0);

    /// A callback function that is called when a collision occurs, this plays out custom functions on collisions.
    std::function<void(PhysicsObject*)> collisionCallback;

    // Position 
    glm::vec2 getPosition() const { return m_position; }
    void setPosition(glm::vec2 position) { m_position = position; }

    glm::vec2 getLocalX() { return m_localX; }
    glm::vec2 getLocalY() { return m_localY; }
    /// Gets the world position using local positions multiplied against a current position
    glm::vec2 toWorld(glm::vec2 pos) { return m_position + m_localX * pos.x + m_localY * pos.y; }

    void setVelocity(glm::vec2 velocity) { m_velocity = velocity; }
    glm::vec2 getVelocity() const { return m_velocity; }
    /// Sets Velocity to 0
    void resetVelocity() { m_velocity = glm::vec2(0, 0); }

    // Angular Velocity
    float getAngularVelocity() { return m_angularVelocity; }

    float getOrientatation() const { return m_orientation; }

    /// Calculates the Kinetic Energy using the Kinetic Energy Formula
    /// E(k) = 1/2 * (Mass * (velocity dot velocity) + moment of inertia * angular velocity^2;
    float getKineticEnergy();
    /// Calculates the Potential Gravitational Energy using the Gravitational Potential Energy Formula
    /// E(g) = -Mass *(Gravity dot Position)
    float getPotentialEnergy();
    
    // Possible Kinematic Effectors

    bool getKinematic() { return m_isKinematic; }
    void setKinematic(bool state) { m_isKinematic = state; }
    void swapKinematicState() { m_isKinematic = !m_isKinematic; }

    void setMass(float mass) { m_mass = mass; }

    /// Returns Mass of object but if the object is Kinematic it will return "infinite" mass to simulate an unmovable object
    //float getMass() { return m_isKinematic ? INT_MAX : (m_lockPosition ? INT_MAX : m_mass); }
    float getMass() { return m_isKinematic || m_actLikeKin ? INT_MAX : m_mass; }

    // moment
    /// Returns Moment of object but if the object is Kinematic it will return "infinite" moment to simulate an unmovable object
   //float getMoment() { return m_isKinematic ? INT_MAX : (m_lockPosition ? INT_MAX : m_moment); }
    float getMoment() { return m_isKinematic || m_actLikeKin ? INT_MAX : m_moment; }

    void setLockPosition(bool state) { m_lockY, m_lockX = state; }
    void setLockX(bool state) { m_lockX = state; }
    void setLockY(bool state) { m_lockY = state; }
    bool isPositionLocked() { return (m_lockY && m_lockX) ? true : false; }
    void setLockRotation(bool state) { m_lockRotation = state; }

    // linear and angular drag
    float getLinearDrag() const { return m_linearDrag; }
    float getAngularDrag() const { return m_angularDrag; }

    void setAngularDragState(bool state) { m_hasAngularDrag = state; }
    void setLinearDragState(bool state) { m_hasLinearDrag = state; }
    void setActKin(bool state) { m_actLikeKin = state; }

    void setOrientation(float orientation) { m_orientation = orientation; }
    float getOrientation() const { return m_orientation; }

protected:
    /// the Global Position of the Rigidbody within the main scene expressed as a Vector2
    glm::vec2 m_position;

    /// the Velocity is the change in position over time and applies change to the position
    /// https://www.khanacademy.org/science/physics/one-dimensional-motion/displacement-velocity-time/a/what-is-velocity
    glm::vec2 m_velocity;

    /// Mass is the amount of matter within an object, it is the measure of an object's resistence to accelleration. 
    /// Mass is not the same as weight but commonly determined through measuring the object's weight.
    /// Mass is determined by a float.
    float m_mass;

    /// Since this is a 2D Enging only a single float is needed to represent orientation
    /// Orientation stores how much an object has rotated in RADIANS
    float m_orientation;


    /// Kinetic Energy is the energy an object has because of its motion, E(k) = 1/2 * mass * (velocity * velocity)
    /// https://www.khanacademy.org/science/physics/work-and-energy/work-and-energy-tutorial/a/what-is-kinetic-energy
    float m_kineticEnergy = 0;

    /// Potential Energy is the stored energy of position possessed by an object, Potential Energy is calculated by E(gp) = mass * gravity * height 
    /// https://www.physicsclassroom.com/class/energy/Lesson-1/Potential-Energy
    float m_potentialEnergy = 0;

    /// Angular Velocity is vector measure of the rotation Rate of an object expressed as a float
    /// https://byjus.com/physics/angular-velocity/#:~:text=In%20physics%2C%20we%20define%20angular,or%20revolves%20about%20an%20axis.
    float m_angularVelocity; 

    /// Moment refers to the concept of the Moment of Inertia. 
    /// The Moment of Inertia (moment) is used to calculate Angular Velocity and is best described as 'how hard it is to rotate an object around a point'
    /// The Moment of Inertia differs depending on the shape and fill of the object.
    float m_moment; // moment of Inertia

    /// Linear Drag is the drag / slowing of an objects linear velocity.
    /// Linear drag is represented as a float, the Recommended Drag is 0.3.
    float m_linearDrag = 0.3f;

    /// Angular Drag is the drag / slowing of an objects rotational velocity.
    /// Angular drag is represented as a float, the Recommended Drag is 0.3.
    float m_angularDrag = 0.3f;

    /// Minimum Linear Threshold reffers to the slowest velocity the object can be moving linearly until its linear velocity is set to 0 (to stop it moving).
    /// Minimum Linear Threshold is represented as a float, the recommended threshold is 0.01.
    const float MIN_LINEAR_THRESHOLD = 0.01f;

    /// Minimum Angular Threshold reffers to the slowest velocity the object can be moving rotationally until its angular velocity is set to 0 (to stop it moving).
    /// Minimum Angular Threshold is represented as a float, the recommended threshold is 0.01.
    const float MIN_ANGULAR_THRESHOLD = 0.01f;

    /// isKinematic declares whether Rigidbody acts as a Kinematic or Rigidbody object.
    bool m_isKinematic;


    // Local X and Y 

    /// Local X stores the local X axes of Box Objects based on the angle of rotation.
    glm::vec2 m_localX;

    /// Local Y stores the local Y axes of Box Objects based on the angle of rotation.
    glm::vec2 m_localY;


    bool m_lockRotation = false;
    bool m_lockX = false;
    bool m_lockY = false;

    bool m_hasAngularDrag = true;
    bool m_hasLinearDrag = true;

    bool m_actLikeKin = false;

};

