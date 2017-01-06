#ifndef ENEMYMESSAGE_H
#define ENEMYMESSAGE_H

#include "BaseMessage.h"
#include "GameEntity.h"

class EnemyMessage : public BaseMessage {
public:
	EnemyMessage(GameEntity* entity);
	~EnemyMessage(void);

	MessageType	getMessageType() const { return m_type; }
	GameEntity* getEntity() { return m_entity; }

private:
	MessageType m_type;
	GameEntity* m_entity;
};

#endif