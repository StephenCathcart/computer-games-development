#include "PickUpMessage.h"


PickUpMessage::PickUpMessage(GameEntity* pickUpEntity, GameEntity* enemyEntity) {
	m_type = MessageType::PICKUP;
	m_pickUpEntity = pickUpEntity;
	m_enemyEntity = enemyEntity;
}


PickUpMessage::~PickUpMessage(void) {
}
