#ifndef GAME_H
#define GAME_H

#include "GameResourceManager.h"
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include "GameSystemManager.h"
#include "GameEntityManager.h"

/// <summary>
/// The main game class.
/// Responsible for initialising all managers/rersources, 
/// running the main loop and cleaning up on shutdown.
/// </summary>
class Game {
public:
	~Game(void);
	Game(void);

	void run();

private:
	static const float	GAME_Z_DISTANCE;

	sf::RenderWindow*		m_window;
	GameResourceManager*	m_resourceManager;
	GameSystemManager*		m_systemManager;
	GameEntityManager*		m_entityManager;

	void					initWindow();
	void					initResources();
	void					initSystems();
	void					initEntities();
	void					shutdown();
};

#endif