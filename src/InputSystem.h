#ifndef INPUTSYSTEM_H
#define INPUTSYSTEM_H

#include "GameSystem.h"
#include <SFML/Graphics.hpp>

/// <summary>
/// System responsible for retriving player input and sending
/// input messages to other systems.
/// </summary>
class InputSystem : public GameSystem {
public:
	InputSystem(sf::RenderWindow &window);
	~InputSystem(void);

	void update(float delta, GameResourceManager& resourceManager, GameEntityManager& entityManager, GameSystemManager& systemManager);
	void handleKeyboard(GameSystemManager& systemManager, sf::Event& e);
	void handleJoypad(GameSystemManager& systemManager, sf::Event& e);
	void receiveMessage(BaseMessage* msg);

protected:
	sf::RenderWindow &m_window;

	static const float JOY_THRESHOLD;
};

#endif