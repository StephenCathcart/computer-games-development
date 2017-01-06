#include "BulletMessage.h"

BulletMessage::BulletMessage(GameEntity* entity) {
	m_type = MessageType::BULLET;
	m_entity = entity;
}


BulletMessage::~BulletMessage(void) {
}
