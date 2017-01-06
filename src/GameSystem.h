#ifndef GAMESYSTEM_H
#define GAMESYSTEM_H

#include "BaseMessage.h"
#include "GameResourceManager.h"
#include "GameEntityManager.h"

// Forward declaration to avoid circular dependacy, 
// needed for messaging system.
class GameSystemManager;

/// <summary>
/// The abstract GameSystem class. Used for game logic over GameEntities.
/// All Systems must implement the receivedMessage function.
/// </summary>
class GameSystem {
public:
	GameSystem(void);
	~GameSystem(void);

	virtual void update(float delta, GameResourceManager& resourceManager, GameEntityManager& entityManager, GameSystemManager& systemManager) = 0;
	virtual void receiveMessage(BaseMessage* msg) = 0;
};

#endif