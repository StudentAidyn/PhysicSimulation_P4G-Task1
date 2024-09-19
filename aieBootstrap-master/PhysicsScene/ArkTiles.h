#pragma once
#include <glm/glm.hpp>
#include "Tile.h"
#include <vector>
class PhysicsSceneApp;

/// ArkTiles is a singulton class that instantiates the tiles and places them within formations to use with the Arkanoid Style gameplay
class ArkTiles
{
public:
	/// Basic Constructor - creates instance of ArkTiles if their isn't an ArkTiles present.
	ArkTiles();
	/// Destructor - deletes / clears vector list -> m_tiles
	~ArkTiles();

	/// Deletes and Clears the m_tiles Vector List 
	void emptyVector();

	/// Build Creates the tile Formation
	/// 
	/// Build uses switch cases and a vector list of strings to create the tile formation. 
	/// Build looks through the strings taking the char values and sets them within a double pointer 
	/// and using the switch cases calls makeTile() to set the correct type of tile, 
	/// at the end of the tile formation the double pointer is deleted.
	/// 
	/// @param app is the main PhysicsSceneApp and is passed through to keep continuity
	/// @param position is the global position the tile formation should be created
	/// @param spacing is the spacing between tiles while they are being generated
	/// @param strings is the vector list of strings that is used to assign tiles
	std::vector<Tile*> Build(PhysicsSceneApp* app, glm::vec2 position, float spacing, std::vector<std::string>& strings);
	/// Creates tiles with the same instruction but differing attribution values including points, health and colour.
	/// 
	/// @param tile is the current tile being modified and instantiated
	/// @param pos is the position the tile is instantiated to
	/// @param points is the total points of the tile being created
	/// @param hp is the total hitpoints of the tile, this value only reduces by one when hit
	/// @param colour is the colour of the tile. 
	void makeTile(Tile* tile, glm::vec2 pos, int points, int hp, glm::vec4 colour);

	void setScene(PhysicsSceneApp* scene) { m_scene = scene; }
	PhysicsSceneApp* getScene() { return m_scene; }
	/// returns the current singulton instance of ArkTiles, if there isn't an instance then it creates one.
	static ArkTiles* getInstance();
	/// Deletes ArkTiles singulton instance setting it to nullptr.
	void destroyArkTiles();
protected:
	static ArkTiles* arkInstance_;
private:
	PhysicsSceneApp* m_scene;
	std::vector<Tile*> m_tiles;
};


// release AND debug won't load text when executed using .executable/ build