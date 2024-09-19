#include "Box.h"

Box::Box(float x, float y, float width, float height, float mass, glm::vec4 colour) :
	Rigidbody(BOX, glm::vec2(x, y), 0, glm::vec2(0), 0, mass, 1, (1.0f / 12.0f * mass * (width) * (height))) {
	m_extents = glm::vec2(width / 2, height / 2);
	m_colour = colour;
}

Box::Box(glm::vec2 position, glm::vec2 extents, float orientation, glm::vec2 velocity, float angularVelocity, float mass, float elasticity, glm::vec4 colour) :
	Rigidbody(BOX, position, orientation, velocity, angularVelocity, mass, elasticity, (1.0f / 12.0f * mass * (extents.x * 2) * (extents.y * 2)) ) {
	m_extents = extents;
	m_colour = colour;
}

Box::Box(float x, float y, float width, float height, float orientation, glm::vec2 velocity, float angularVelocity, float mass, float elasticity, glm::vec4 colour) :
	Rigidbody(BOX, glm::vec2(x, y), orientation, velocity, angularVelocity, mass, elasticity, (1.0f / 12.0f * mass * width * height)) {
	m_extents = glm::vec2(width/2, height/2);
	m_colour = colour;
}

Box::~Box() {

}




void Box::draw() {
	// m_extents, m_colour, &transform);
	// draw using local axes
	glm::vec2 p1 = m_position - getLocalX() * m_extents.x - getLocalY() * m_extents.y;
	glm::vec2 p2 = m_position + getLocalX() * m_extents.x - getLocalY() * m_extents.y;
	glm::vec2 p3 = m_position - getLocalX() * m_extents.x + getLocalY() * m_extents.y;
	glm::vec2 p4 = m_position + getLocalX() * m_extents.x + getLocalY() * m_extents.y;
	//aie::Gizmos::add2DTri(p1, p2, p4, (m_isKinematic ? glm::vec4(1, 1, 1, 1) : m_colour));
	//aie::Gizmos::add2DTri(p1, p4, p3, (m_isKinematic ? glm::vec4(1, 1, 1, 1) : m_colour));	
	aie::Gizmos::add2DTri(p1, p2, p4,  m_colour);
	aie::Gizmos::add2DTri(p1, p4, p3, m_colour);
}


// check if any of the other box's corners are inside this box
bool Box::checkBoxCorners(Box& box, glm::vec2& contact, int& numContacts, float& pen, glm::vec2& edgeNormal) {
	// creates mins and maxes for x and y axes
	float minX, maxX, minY, maxY;
	// sets box width and height
	float boxW = box.getExtents().x * 2;
	float boxH = box.getExtents().y * 2;
	// number of local contacts
	int numLocalContacts = 0;
	// location of contacts
	glm::vec2 localContact(0, 0);
	// sets first to true to allow the first calculation of p0/min-max values
	bool first = true;
	// loop over all corners of the other box
	for (float x = -box.getExtents().x; x < boxW; x += boxW)
	{
		for (float y = -box.getExtents().y; y < boxH; y += boxH)
		{
			// get the position in worldspace
			glm::vec2 p = box.getPosition() + x * box.getLocalX() + y * box.getLocalY();
			// get the position in our box's space
			glm::vec2 p0(glm::dot(p - m_position, getLocalX()),
				glm::dot(p - m_position, getLocalY()));
			// update the extents in each cardinal direction in our box's space
			// (ie extents along the separating axes)
			if (first || p0.x < minX) minX = p0.x;
			if (first || p0.x > maxX) maxX = p0.x;
			if (first || p0.y < minY) minY = p0.y;
			if (first || p0.y > maxY) maxY = p0.y;
			// if this corner is inside the box, add it to the list of contact points
			if (p0.x >= -m_extents.x && p0.x <= m_extents.x &&
				p0.y >= -m_extents.y && p0.y <= m_extents.y)
			{
				numLocalContacts++;
				localContact += p0;
			}
			first = false;
		}
	}
	// if we lie entirely to one side of the box along one axis, we've found a separating
		// axis, and we can exit
	if (maxX <= -m_extents.x || minX >= m_extents.x ||
		maxY <= -m_extents.y || minY >= m_extents.y)
		return false;
	if (numLocalContacts == 0)
		return false;
	bool res = false;
	contact += m_position + (localContact.x * getLocalX() + localContact.y * getLocalY()) /
		(float)numLocalContacts;
	numContacts++;
	// find the minimum penetration vector as a penetration amount and normal
	float pen0 = m_extents.x - minX;
	if (pen0 > 0 && (pen0 < pen || pen == 0)) {
		edgeNormal = getLocalX();
		pen = pen0;
		res = true;
	}
	pen0 = maxX + m_extents.x;
	if (pen0 > 0 && (pen0 < pen || pen == 0)) {
		edgeNormal = -getLocalX();
		pen = pen0;
		res = true;
	}
	pen0 = m_extents.y - minY;
	if (pen0 > 0 && (pen0 < pen || pen == 0)) {
		edgeNormal = getLocalY();
		pen = pen0;
		res = true;
	}
	pen0 = maxY + m_extents.y;
	if (pen0 > 0 && (pen0 < pen || pen == 0)) {
		edgeNormal = -getLocalY();
		pen = pen0;
		res = true;
	}
	return res;
}