#include "Game.h"

/// <summary>
/// Main creates an instance of game and runs it.
/// 
/// ** README: GAME ENGINE EXPLAINATION ***************************************
/// * The game engine runs using a component-entity-system design and
///	* uses a messaging system to send data between systems.
/// * Entities are made up of various components. They are then added to the
/// * GameEntityManager. The GameSystems have different responsibilites such
/// * as updating the physics or rendering the entities. The GameSystems choose
/// * what entities manipulate depending on what components they have attached.
/// * The GameSystems may pass data between them using various GameMessages.
/// ***************************************************************************
///
/// ** BASE CLASSES ***********************************************************
/// * The base "Component" class is GameComponent.
/// * The base "Entity" class is GameEntity.
/// * The base "System" class is GameSystem.
/// * The base "Message" class is GameMessage.
/// ***************************************************************************
/// 
/// ** MANAGER CLASSES ********************************************************
/// * The "GameResourceManager" is responsible for Mesh, Shader and Audio data.
/// * The "GameSystemManager" is responsible for running all GameSystems
/// * The "GameEntityManager" is responsible for all GameEntitys.
/// ***************************************************************************
///
/// ** PATH DATA **************************************************************
/// * The "Path" class is used to store the path strings for all resources such
/// * as .OBJ, texture, sound, font and shader files.
/// ***************************************************************************
///
///	Please note that all of the GameSystems' "logic "files are a bit messy at
/// the moment as I have been mainly concentrating on the engine rather than
//	gameplay.
///
/// </summary>
/// <returns></returns>
int main() {
	Game *game = new Game;
	game->run();

	delete game;
	return 0;
}
