#include "BulletCollisionMessage.h"


BulletCollisionMessage::BulletCollisionMessage(GameEntity* entityBullet, GameEntity* entityEnemy) {
	m_type = MessageType::BULLETCOLLISION;
	m_entityBullet = entityBullet;
	m_entityEnemy = entityEnemy;
}


BulletCollisionMessage::~BulletCollisionMessage(void) {
}
