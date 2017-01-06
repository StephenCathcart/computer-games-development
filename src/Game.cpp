#include "Game.h"
#include "glm\glm.hpp"
#include "Paths.h"
#include "Settings.h"
#include "GameEntity.h"
#include "InputSystem.h"
#include "PhysicsSystem.h"
#include "CollisionSystem.h"
#include "LogicSystem.h"
#include "RenderingSystem.h"

// As game is capped to XY coordinates, set default Z value
const float	Game::GAME_Z_DISTANCE = -200.0f;

/// <summary>
/// Initializes a new instance of the <see cref="Game"/> class.
/// </summary>
Game::Game(void) {
}

/// <summary>
/// Memory cleaned up in shutdown function.
/// </summary>
Game::~Game(void) {
}

/// <summary>
/// Runs the game loop.
/// </summary>
void Game::run() {
	initWindow();
	Settings::loadHighscores();
	initResources();
	initSystems();
	initEntities();

	sf::Clock clock;
	while (m_systemManager->getIsActive()) {
		float delta = clock.restart().asMilliseconds();
		m_systemManager->update(delta, *m_resourceManager, *m_entityManager);
	}
	shutdown();
}

/// <summary>
/// Initializes the window.
/// </summary>
void Game::initWindow() {
	// Initialise window
	m_window = new sf::RenderWindow(sf::VideoMode(1280, 720), "SHMUP Game", sf::Style::Fullscreen, sf::ContextSettings(32));

	// Enable VSync
	m_window->setVerticalSyncEnabled(true);

	// Hide mouse
	m_window->setMouseCursorVisible(false);
}

/// <summary>
/// Initializes the resources.
/// </summary>
void Game::initResources() { 
	// Initialise resource manager
	m_resourceManager = new GameResourceManager;
}

/// <summary>
/// Initializes the systems.
/// </summary>
void Game::initSystems() {
	// Initialise system manager responsible for input, logic, physics and render systems
	m_systemManager = new GameSystemManager(*m_window);

	// Load Systems
	m_systemManager->addSystem(new InputSystem(*m_window));
	m_systemManager->addSystem(new LogicSystem(*m_window));
	m_systemManager->addSystem(new CollisionSystem);
	m_systemManager->addSystem(new PhysicsSystem);
	m_systemManager->addSystem(new RenderingSystem(*m_window));
}

/// <summary>
/// Initializes the entities.
/// </summary>
void Game::initEntities() {
	// Initialise entity manager
	m_entityManager = new GameEntityManager;
}

/// <summary>
/// Clean up memory.
/// </summary>
void Game::shutdown() {
	delete m_window;
	delete m_systemManager;
	delete m_entityManager;
	delete m_resourceManager;
}
