#ifndef PHYSICSSYSTEM_H
#define PHYSICSSYSTEM_H

#include "GameSystem.h"
#include "PlayerComponent.h"
#include "VelocityComponent.h"

/// <summary>
/// System responsible for updating the velocities and positions of
/// entities with Position & Velocity Components.
/// </summary>
class PhysicsSystem : public GameSystem {
public:
	PhysicsSystem(void);
	~PhysicsSystem(void);

	void update(float delta, GameResourceManager& resourceManager, GameEntityManager& entityManager, GameSystemManager& systemManager);
	void receiveMessage(BaseMessage* msg);

	void handlePlayerPhysics(BaseMessage* msg);
	void handleBulletPhysics(BaseMessage* msg);
	void handlePickUpPhysics(BaseMessage* msg);
	void handleEnemyPhysics(BaseMessage* msg);
	void setPlayerY(PlayerComponent* p, VelocityComponent* v, bool isInclining, bool isDeclining, float vY);
	void setPlayerX(VelocityComponent* v, float vX);

	float calculateDistance(float x1, float y1, float x2, float y2);

private:
	GameEntityManager*	m_entityManager;
	static const float	ACTIVE_VELOCITY;
	static const float	INACTIVE_VELOCITY;
	static const float	DAMPENING;
	static const float	MAX_ROTATION;
	static const float	ROTATION_STEP;
	static const float	BULLET_VELOCITY;
	static const float	SPRING_AMOUNT;
	static const float	CRATE_SPEED;
	static const float  CRATE_X;
	static const float  CRATE_Y;
	static const float  ENEMY_SPEED;
	static const float	MAX_X;
	static const float	MAX_Y;

	static float timer;

	void	updateStart();
	void	updateRun(float delta);
	void	updatePause();
	void	updateCredits();
	void	updateGameOver();
};

#endif
