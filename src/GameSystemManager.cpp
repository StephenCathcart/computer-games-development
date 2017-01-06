#include "GameSystemManager.h"

/// <summary>
/// Initializes a new instance of the <see cref="GameSystemManager"/> class.
/// </summary>
/// <param name="window">The window.</param>
GameSystemManager::GameSystemManager(sf::RenderWindow &window) {
	m_isActive = true;
}

/// <summary>
/// Finalizes an instance of the <see cref="GameSystemManager"/> class.
/// </summary>
GameSystemManager::~GameSystemManager(void) {
	for (int i = 0; i < m_systems.size(); i++) {       
		delete m_systems[i];
    }
}

/// <summary>
/// Updates GameSystems.
/// </summary>
/// <param name="delta">The delta.</param>
/// <param name="entityManager">The entity manager.</param>
void GameSystemManager::update(float delta, GameResourceManager& resourceManager, GameEntityManager& entityManager) {
	for (int i = 0; i < m_systems.size(); i++) {       
		m_systems[i]->update(delta, resourceManager, entityManager, *this);
    }    
}

/// <summary>
/// Sends GameSystem Messages.
/// </summary>
/// <param name="msg">The MSG.</param>
void GameSystemManager::sendMessage(BaseMessage* msg) {
	for (int i = 0; i < m_systems.size(); i++) {       
		m_systems[i]->receiveMessage(msg);
    }    
}