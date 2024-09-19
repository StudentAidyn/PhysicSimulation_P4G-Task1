#include "PhysicsSceneApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include <iostream>
#include "ArkTiles.h"
#include "Tile.h"


PhysicsSceneApp::PhysicsSceneApp() {

}

PhysicsSceneApp::~PhysicsSceneApp() {
	
	emptyVector();
	//delete pilot;
	pilot = nullptr;

	//delete ball;
	ball = nullptr;

	//delete floor;
	floor = nullptr;
	delete m_physicsScene;
	m_physicsScene = nullptr;

	m_font = nullptr;
	m_2dRenderer = nullptr;

	aie::Gizmos::destroy();
	ArkTiles::getInstance()->destroyArkTiles();



}

// On startup the renderer is instantiated and physicsScene, setting the timeStep to 0.01f
bool PhysicsSceneApp::startup() {
	
	// increase the 2d line count to maximize the number of objects we can draw
	aie::Gizmos::create(255U, 255U, 65535U, 65535U); // this is created, is destroyed now in destructor

	// Instantiates new Renderer
	m_2dRenderer = new aie::Renderer2D();

	/* Instantiates new Font, file location is using $(SolutionDir)bin\ */
	m_font = new aie::Font("./font/consolas.ttf", 32);

	// Instantiates new PhysicsScene class and sets physics based timestep
	m_physicsScene = new PhysicsScene();
	m_physicsScene->setTimeStep(0.01f);

	// Game Scene Setup
	gameSetup();

	// calls next level to start generate the first level
	nextLevel();

	return true;
}

void PhysicsSceneApp::gameSetup() {
	// Setting up the player
	pilot = new Box(0, -50, 10, 5, 0, glm::vec2(0), 0, 10, 1, glm::vec4(0, 1, 0.1f, 1));
	pilot->setKinematic(true);
	m_physicsScene->addActor(pilot);

	// FLOOR and ROOF
	floor = new Plane(glm::vec2(0, 1), -64, 0); // floor is set off screen and is a global variable
	m_physicsScene->addActor(floor);
	Plane* roof = new Plane(glm::vec2(0, -1), -56, 0);
	m_physicsScene->addActor(roof);

	// LEFT and RIGHT WALLS
	Plane* wallL = new Plane(glm::vec2(1, 0), -56, 0);
	m_physicsScene->addActor(wallL);
	Plane* wallR = new Plane(glm::vec2(-1, 0), -56, 0);
	m_physicsScene->addActor(wallR);
}

void PhysicsSceneApp::shutdown() {
	delete m_font;
	delete m_2dRenderer;
}

// update calls phyisic scene update and draw passing through delta time to update
void PhysicsSceneApp::update(float deltaTime) {
	aie::Gizmos::clear();

	m_physicsScene->update(deltaTime);
	m_physicsScene->draw();
	if (checkActiveTiles()) {
		m_currentLevel++;
		delete ball;
		nextLevel();
	}
	if (m_lives <= 0) {
		if (m_points > m_highScore) { m_highScore = m_points; }
		m_currentLevel = 0;
		m_points = 0;
		m_lifeUpCounter = 0;
		m_lives = DEFAULT_LIVES;
		nextLevel();
	}
	if (m_lifeUpCounter >= POINTS_LIFEUP) {
		m_lives++;
		m_lifeUpCounter = m_points % 10000;
	}

	controls(deltaTime);
}

void PhysicsSceneApp::controls(float deltaTime) {

	// input singulton
	aie::Input* input = aie::Input::getInstance();

	// Player can fire out a ball in the direction they are facing
	if (input->isKeyDown(aie::INPUT_KEY_SPACE) && !noBall) {
		ball = new Sphere(pilot->getPosition() + glm::vec2(0, 1.5), 1.5f, 0, glm::vec2(m_ballSpeed * -1 * pilot->getOrientation(), m_ballSpeed), 0, 5, 1, glm::vec4(0, 0.9f, 0.2f, 1));
		ball->setLinearDragState(false);
		ball->collisionCallback = [=](PhysicsObject* other) {
			if (other == floor) {
				ball->setForRemoval();
				m_lives--;
				if (m_lives > 0) {
					noBall = false;
				}
				delete ball;
			}
		};
		m_physicsScene->addActor(ball);
		noBall = true;
	}


	// ROTATIONAL MOVEMENT
	if (input->isKeyDown(aie::INPUT_KEY_J)) {
		if (pilot->getOrientation() <= m_maxRotation) { pilot->setOrientation(pilot->getOrientation() + m_rotationalSpeed * deltaTime); }
		if (pilot->getOrientation() > m_maxRotation) { pilot->setOrientation(m_maxRotation); }
	}

	if (input->isKeyDown(aie::INPUT_KEY_L)) {
		if (pilot->getOrientation() >= -m_maxRotation) { pilot->setOrientation(pilot->getOrientation() - m_rotationalSpeed * deltaTime); }
		if (pilot->getOrientation() < -m_maxRotation) { pilot->setOrientation(-m_maxRotation); }
	}

	// RESETS ORIENTATION
	if (input->isKeyDown(aie::INPUT_KEY_K)) {
		pilot->setOrientation(0);
	}

	// LEFT AND RIGHT MOVEMENT 
	if (input->isKeyDown(aie::INPUT_KEY_A)) {
		pilot->setPosition(glm::vec2(pilot->getPosition().x - m_movementSpeed * deltaTime, pilot->getPosition().y));
	}

	if (input->isKeyDown(aie::INPUT_KEY_D)) {
		pilot->setPosition(glm::vec2(pilot->getPosition().x + m_movementSpeed * deltaTime, pilot->getPosition().y));
	}

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE)) {
		quit();
	}
		
}

bool PhysicsSceneApp::checkActiveTiles() {
	for (Tile* tile : m_levelTiles) {
		if (tile->getState()) {
			return false;
		}
	}
	return true;
}

void PhysicsSceneApp::nextLevel() {
	emptyVector();
	m_physicsScene->removeActor(ball);
	noBall = false;
	TileSequenceBuilder(m_currentLevel);
}

Rigidbody* PhysicsSceneApp::closestObject(Rigidbody* body, ShapeType type) {
	// returns if object type is rigidbody
	if (type < 1) return nullptr;
	// creates vector list to store all objects of said type
	std::vector<PhysicsObject*> objects = m_physicsScene->getShapesOfType(type);
	// checks if the vector list is empty
	if (objects.empty()) {
		std::cout << "No Shapes of type: " << type << " Found!" << std::endl; return nullptr;
	}
	// sets up loop variables
	// declare the first pass and define the closest value
	bool first = true;
	float closest = 0;
	Rigidbody* object;
	for (auto typeObject : objects) {
		Rigidbody* obj = dynamic_cast<Rigidbody*>(typeObject);
		float dist = glm::distance(body->getPosition(), obj->getPosition());
		if (first || dist < closest) {
			object = obj;
			closest = dist;
			first = false;
		}
	}
	return object;
}

void PhysicsSceneApp::emptyVector() {
	for (auto tile : m_levelTiles) {
		if (tile->getState()) { m_physicsScene->removeActor(tile); }
		delete tile;
		tile = nullptr;
	}
	m_levelTiles.erase(m_levelTiles.begin(), m_levelTiles.end());
} 


void PhysicsSceneApp::TileSequenceBuilder(int lvl) {
	m_levelTiles = ArkTiles::getInstance()->Build(this, glm::vec2(0, 0), 7.f, levelLoader(lvl));
}

// Draw is called to add to the screen
void PhysicsSceneApp::draw() {
	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	char level[32];
	sprintf_s(level, 32, "Level %i", m_currentLevel + 1);
	m_2dRenderer->drawText(m_font, level, 0, 720 - 32 * 4);

	char highScore[32];
	sprintf_s(highScore, 32, "High Score %i", m_highScore);
	m_2dRenderer->drawText(m_font, highScore, 0, 720 - 96);

	char points[32];
	sprintf_s(points, 32, "Points %i", m_points);
	m_2dRenderer->drawText(m_font, points, 0, 720 - 64);

	char lives[32];
	sprintf_s(lives, 32, "Lives %i", m_lives);
	m_2dRenderer->drawText(m_font, lives, 0, 720 - 32);

	//aie::Gizmos::draw2D((float)getWindowWidth(), (float)getWindowHeight());
	// draw your stuff here!
	static float aspectRatio = 16 / 9.f;
	aie::Gizmos::draw2D(glm::ortho<float>(-100, 100, -100 / aspectRatio, 100 / aspectRatio, -1.0f, 1.0f));

	// done drawing sprites
	m_2dRenderer->end();
}

std::vector<std::string> PhysicsSceneApp::levelLoader(int level) {
	std::vector<std::string> lvl;
	switch (level) {
	case 0:
		lvl.push_back("33333333333333");
		lvl.push_back("22222222222222");
		lvl.push_back("11111111111111");
		lvl.push_back("11111111111111");
		lvl.push_back("00000000000000");
		lvl.push_back("00000000000000");
		break;
	case 1:
		lvl.push_back("00000000000000");
		lvl.push_back("..............");
		lvl.push_back("11111111111111");
		lvl.push_back("..............");
		lvl.push_back("22222222222222");
		lvl.push_back("..............");
		lvl.push_back("11111111111111");
		lvl.push_back("..............");
		lvl.push_back("00000000000000");
		break;
	case 2:
		lvl.push_back(".000000.");
		lvl.push_back("00000000");
		lvl.push_back("02200220");
		lvl.push_back("02200220");
		lvl.push_back("00000000");
		lvl.push_back("01000010");
		lvl.push_back("00111100");
		lvl.push_back("00000000");
		lvl.push_back(".000000.");
		break;
	case 3:
		lvl.push_back(".....333333....");
		lvl.push_back("....33333333...");
		lvl.push_back("..0.31111113.0.");
		lvl.push_back("..0.11111111.0.");
		lvl.push_back("..001030030100.");
		lvl.push_back("..001010010100.");
		lvl.push_back("...00000000001.");
		lvl.push_back("...33001100331.");
		lvl.push_back(".11111000033111");
		lvl.push_back("110111133333011");
		lvl.push_back("100011011330001");
		lvl.push_back("110111031111000");
		lvl.push_back("11011101133330.");
		lvl.push_back("111111033333...");
		lvl.push_back(".000001..111...");
		lvl.push_back("....111........");
		break;
	case 4:
		lvl.push_back("...11111....");
		lvl.push_back("..111111111.");
		lvl.push_back("..2220020...");
		lvl.push_back(".2020002000.");
		lvl.push_back(".20220002000");
		lvl.push_back(".2200002222.");
		lvl.push_back("...0000000..");
		lvl.push_back("..221222....");
		lvl.push_back(".2221221222.");
		lvl.push_back("222211112222");
		lvl.push_back("002101101200");
		lvl.push_back("000111111000");
		lvl.push_back("001111111100");
		lvl.push_back("..111..111..");
		lvl.push_back(".222....222.");
		lvl.push_back("222......222");
		break;
	default:
		lvl.push_back(".000000.");
		lvl.push_back("00000000");
		lvl.push_back("00...000");
		lvl.push_back("00..0.00");
		lvl.push_back("00.0..00");
		lvl.push_back("000...00");
		lvl.push_back("00000000");
		lvl.push_back(".000000.");
	}
	return lvl;
}



/*
SCORE + HIGH SCORE
CHANGE ANGLE OF BALL FIRE
GAME LOOP
*/





