#ifndef BULLETMESSAGE_H
#define BULLETMESSAGE_H

#include "BaseMessage.h"
#include "GameEntity.h"

class BulletMessage : public BaseMessage {
public:
	BulletMessage(GameEntity* entity);
	~BulletMessage(void);

	MessageType	getMessageType() const { return m_type; }
	GameEntity* getEntity() { return m_entity; }

private:
	MessageType m_type;
	GameEntity* m_entity;
};

#endif
