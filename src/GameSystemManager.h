#ifndef GAMESYSTEMMANAGER_H
#define GAMESYSTEMMANAGER_H

#include "BaseMessage.h"
#include "GameEntityManager.h"
#include "GameSystem.h"
#include <SFML/Graphics.hpp>

using std::vector;

/// <summary>
///  Container for instances GameSystems
/// </summary>
class GameSystemManager {
public:
	GameSystemManager(sf::RenderWindow &window);
	~GameSystemManager(void);

	void update(float delta, GameResourceManager& resourceManager, GameEntityManager& manager);
	void sendMessage(BaseMessage* msg);
	bool getIsActive() const { return m_isActive; }
	void setIsActive(bool isActive) { m_isActive = isActive; }

	void addSystem(GameSystem* s) { m_systems.push_back(s); }

	bool removeSystem(GameSystem* s) {
		for (vector<GameSystem*>::iterator i = m_systems.begin(); i != m_systems.end(); ++i) {
			if(*i == s) {
				delete m_systems[i - m_systems.begin()];
				i = m_systems.erase(i);
				return true;
			}
		}
		return false;
	}

private:
	bool				m_isActive;
	vector<GameSystem*> m_systems;
};

#endif
