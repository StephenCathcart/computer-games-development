#ifndef PICKUPCOLLISIONMESSAGE_H
#define PICKUPCOLLISIONMESSAGE_H

#include "BaseMessage.h"
#include "GameEntity.h"

/// <summary>
/// A PickUp Message holds a pointer to the pick up entity that
/// has been collected;
/// </summary>
class PickUpCollisionMessage : public BaseMessage {
public:
	PickUpCollisionMessage(GameEntity* entity);
	~PickUpCollisionMessage(void);

	MessageType	getMessageType() const { return m_type; }
	GameEntity* getEntity() { return m_entity; }

private:
	MessageType m_type;
	GameEntity* m_entity;
};

#endif