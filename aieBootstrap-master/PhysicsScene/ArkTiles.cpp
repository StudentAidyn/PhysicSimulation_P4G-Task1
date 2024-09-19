#include "ArkTiles.h"
#include "PhysicsSceneApp.h"

ArkTiles* ArkTiles::arkInstance_ = nullptr;
ArkTiles::ArkTiles() {
	if (arkInstance_ == nullptr) { arkInstance_ = this; }
}

ArkTiles::~ArkTiles() {
	emptyVector();
}

void ArkTiles::destroyArkTiles() {
	delete arkInstance_;
	arkInstance_ = nullptr;
}

ArkTiles* ArkTiles::getInstance() {
	if (arkInstance_ == nullptr) { arkInstance_ = new ArkTiles(); }
	return arkInstance_;
}
std::vector<Tile*> ArkTiles::Build(PhysicsSceneApp* scene, glm::vec2 position, float spacing, std::vector<std::string>& strings)
{
	emptyVector();
	/// Declares the number of COLUMNS is the arrays size
	int numColumns = static_cast<int>(strings.size());
	/// Declares the number of ROWS is element 0's Length
	int numRows = static_cast<int>(strings[0].length());

	// setting for PhysicsScene use
	setScene(scene);
	Tile* current;
	float centre = (spacing * (float)numRows) * 0.5f - (spacing * 0.5f);

	Tile** tile = new Tile* [numRows * numColumns];
	for (int i = 0; i < numRows; i++) {
		for (int j = 0; j < numColumns; j++) {
			int x = i;
			int y = numColumns - 1 - j;
			current = tile[x * numColumns + y];

			glm::vec2 tilePos = glm::vec2(position.x - centre + x * spacing, position.y + y * spacing * 0.5f);
			switch (strings[j][i]) {
			case '0':
				makeTile(current, tilePos, 50, 1, glm::vec4(1, 1, 0, 1));
				break;
			case '1':
				makeTile(current, tilePos, 100, 1, glm::vec4(1, 0, 0, 1));
				break;
			case '2':
				makeTile(current, tilePos, 200, 2, glm::vec4(0, 0.2f, 1, 1));
				break;
			case '3':
				makeTile(current, tilePos, 400, 2, glm::vec4(0, 1, 0.1f, 1));
				break;
			default:
				current = nullptr;
			}
			
		}
	}
	delete tile;
	return m_tiles;
}

void ArkTiles::emptyVector() {
	for (auto tile : m_tiles) {
		tile = nullptr;
	}
	m_tiles.erase(m_tiles.begin(), m_tiles.end());
	m_tiles.clear();
}


void ArkTiles::makeTile(Tile* tile, glm::vec2 pos, int points, int hp, glm::vec4 colour) {
	tile = new Tile(pos, points, hp, colour);
	m_scene->m_physicsScene->addActor(tile);
	tile->collisionCallback = [=](PhysicsObject* other) {
		if (other->getShapeID() == SPHERE) {
			// calls hit, hit reduces the health of the tile by 1;

			// std::cout << m_scene->getPoints() << std::endl;
			if (tile->hit() <= 0) {
				tile->setState(false);
				m_scene->AddPoints(tile->getPoints());
				tile->setForRemoval();
			}
		}
		};
	m_tiles.push_back(tile);
}

