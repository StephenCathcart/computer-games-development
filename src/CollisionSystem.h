#ifndef COLLISIONSYSTEM_H
#define COLLISIONSYSTEM_H

#include "GameSystem.h"
/// <summary>
/// System responsible for all collisions between entities. Collision boundry data
/// is calculated in the mesh class on creation.
/// </summary>
class CollisionSystem : public GameSystem {
public:
	CollisionSystem(void);
	~CollisionSystem(void);

	void update(float delta, GameResourceManager& resourceManager, GameEntityManager& entityManager, GameSystemManager& systemManager);
	void receiveMessage(BaseMessage* msg);

	bool checkCollision(GameEntity* player, GameEntity* pickup);

private:
	GameEntityManager*	m_entityManager;
	GameSystemManager*	m_systemManager;

	void	updateStart();
	void	updateRun();
	void	updatePause();
	void	updateCredits();
};

#endif
