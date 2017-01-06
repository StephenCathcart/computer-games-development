#include "BulletComponent.h"

BulletComponent::BulletComponent(bool isActive) {
	m_isActive = isActive;
	m_bulletType = BulletType::LINEAR;
	m_damage	= 1;
}

BulletComponent::~BulletComponent(void) {
}
