#include "PickUpComponent.h"

/// <summary>
/// Initializes a new instance of the <see cref="PickUpComponent"/> class.
/// </summary>
PickUpComponent::PickUpComponent(bool isActive, PickUpType pickUpType) {
	m_isActive = isActive;
	m_pickUpType = pickUpType;
}

/// <summary>
/// Finalizes an instance of the <see cref="PickUpComponent"/> class.
/// </summary>
PickUpComponent::~PickUpComponent(void) {
}
