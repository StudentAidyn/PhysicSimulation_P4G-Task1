#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include "PhysicsScene.h"
#include "Tile.h"
#include <Gizmos.h>
#include <glm/ext.hpp>
#include <vector>
//#include "Pilot.h"

/// PhysicsSceneApp is derived from Application within Bootstrap
/// 
/// PhysicsSceneApp runs the main scene and draws the Gizmo (shapes) to the scene. 
/// The App runs the startup function which adds all initial objects to the scene and sets the PhysicsScene variables timeStep and Gravity. 
/// 
/// Physics Scene App is mainly controlled the Bootstrap Extension.

class PhysicsSceneApp : public aie::Application {
public:
	/// Main PhysicsScene
	PhysicsScene* m_physicsScene;

	/// Constructor
	PhysicsSceneApp();
	/// Virtual Destructor
	virtual ~PhysicsSceneApp();

	/// Sets Up the Physics Based Scene
	/// 
	/// Startup adds all initial/starting objects to the main physics controlled scene and sets the Physics Conditions.
	virtual bool startup();
	/// Shutdown removes deletes memory variables
	virtual void shutdown();

	void gameSetup();

	/// Updates Scene
	/// 
	/// Update clears Gizmo Objects and calls m_physicsScene update and draw methods. 
	/// controls() is called within update to check the current controls.  
	/// @param deltaTime is the change in time between frames
	virtual void update(float deltaTime);


	/// Draws all Gizmo Objects to Screen
	virtual void draw();

	/// Checks for specified inputs and applies actions based on inputs
	void controls(float deltaTime);

	/// WASD Sphere Pilot
	Box* pilot;
	Sphere* ball;
	Plane* floor;

	std::vector<std::string> levelLoader(int level);
	void nextLevel();

	int getPoints() { return m_points; }
	void AddPoints(int points) { m_points += points; m_lifeUpCounter += points; }

	/// using similar input from the softbody builder the breakable blocks are created
	void TileSequenceBuilder(int lvl);

	bool checkActiveTiles();

	/// Finds the closest object of the passed type to the rigidbody
	/// 
	Rigidbody* closestObject(Rigidbody* body, ShapeType type);

	void emptyVector();

protected:
	/// 2D Renderer provided by AIE Bootstrap
	aie::Renderer2D*	m_2dRenderer;
	/// Fonts provided by AIE Bootstrap
	aie::Font*			m_font;


	const int DEFAULT_LIVES = 3;
	int m_lives = DEFAULT_LIVES;
	const int POINTS_LIFEUP = 10000;
	int m_highScore = 0;
	int m_points = 0;
	int m_lifeUpCounter = 0;
	bool noBall = false;

	float m_movementSpeed = 30.0f;
	float m_rotationalSpeed = 5.0f;
	float m_maxRotation = 0.8f;
	float m_ballSpeed = 44.f;

	bool m_powerCharged = false;
	int m_currentLevel = 0;



	std::vector<Tile*> m_levelTiles;
};