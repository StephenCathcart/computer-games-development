#include "MenuItemComponent.h"


MenuItemComponent::MenuItemComponent(void) {
	m_isActive = false;
}

MenuItemComponent::MenuItemComponent(bool isActive) {
	m_isActive = isActive;
}

MenuItemComponent::~MenuItemComponent(void) {
}
