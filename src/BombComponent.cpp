#include "BombComponent.h"

BombComponent::BombComponent(void) {
	m_amount = 0;
}

BombComponent::BombComponent(int amount) {
	m_amount = amount;
}

BombComponent::~BombComponent(void) {
}
