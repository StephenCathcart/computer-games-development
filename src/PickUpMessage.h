#ifndef PICKUPMESSAGE_H
#define PICKUPMESSAGE_H

#include "BaseMessage.h"
#include "GameEntity.h"

class PickUpMessage : public BaseMessage {
public:
	PickUpMessage(GameEntity* pickUpEntity, GameEntity* enemyEntity);
	~PickUpMessage(void);

	MessageType	getMessageType() const { return m_type; }
	GameEntity* getPickUpEntity() { return m_pickUpEntity; }
	GameEntity* getEnemyEntity() { return m_enemyEntity; }

private:
	MessageType m_type;
	GameEntity* m_pickUpEntity;
	GameEntity* m_enemyEntity;
};

#endif

