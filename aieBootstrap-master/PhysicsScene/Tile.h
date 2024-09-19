#pragma once
#include "Box.h"

class Tile : public Box
{
public:
	/// Tile Constructor sets local variables and Calls Box Constructor
	Tile(glm::vec2 position, int points, int Health, glm::vec4 colour);
	/// Default Constructor
	Tile() {}
	/// Destructor
	~Tile() {}
	/*Points, Health, size*/
	/// Lowers Health by 1
	int hit() { m_health--; return m_health; }

	int getPoints() { return m_points; }
	bool getState() { return m_active; }
	void setState(bool state) { m_active = state; }
	
private:
	int m_points = 100;
	int m_health = 1;	
	bool m_active = true;
};

