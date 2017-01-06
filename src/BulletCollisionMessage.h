#ifndef BULLETCOLLISIONMESSAGE_H
#define BULLETCOLLISIONMESSAGE_H

#include "BaseMessage.h"
#include "GameEntity.h"

class BulletCollisionMessage : public BaseMessage {
public:
	BulletCollisionMessage(GameEntity* entityBullet, GameEntity* entityEnemy);
	~BulletCollisionMessage(void);

	MessageType	getMessageType() const { return m_type; }
	GameEntity* getBulletEntity() { return m_entityBullet; }
	GameEntity* getEnemyEntity() { return m_entityEnemy; }

private:
	MessageType m_type;
	GameEntity* m_entityBullet;
	GameEntity* m_entityEnemy;
};

#endif