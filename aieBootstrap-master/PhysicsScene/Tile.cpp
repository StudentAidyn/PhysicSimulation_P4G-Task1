#include "Tile.h"
#include "PhysicsScene.h"

Tile::Tile(glm::vec2 position, int points, int health, glm::vec4 colour) : Box(position.x, position.y, 6, 3.f, 1, colour) {
	m_health = health;
	m_points = points;	
	setKinematic(true);

}
