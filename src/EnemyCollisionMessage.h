#ifndef ENEMYCOLLISIONMESSAGE_H
#define ENEMYCOLLISIONMESSAGE_H

#include "BaseMessage.h"
#include "GameEntity.h"

/// <summary>
/// An Enemy Message holds a pointer to the enemy entity that
/// has been collected;
/// </summary>
class EnemyCollisionMessage : public BaseMessage {
public:
	EnemyCollisionMessage(GameEntity* entity);
	~EnemyCollisionMessage(void);

	MessageType	getMessageType() const { return m_type; }

	GameEntity* getEntity() { return m_entity; };

private:
	MessageType m_type;
	GameEntity* m_entity;
};

#endif

