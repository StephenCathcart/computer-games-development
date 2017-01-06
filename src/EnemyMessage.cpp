#include "EnemyMessage.h"

EnemyMessage::EnemyMessage(GameEntity* entity) {
	m_type = MessageType::ENEMY;
	m_entity = entity;
}

EnemyMessage::~EnemyMessage(void) {
}
