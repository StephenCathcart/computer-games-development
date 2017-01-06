#include "HealthComponent.h"

/// <summary>
/// Initializes a new instance of the <see cref="HealthComponent"/> class.
/// </summary>
HealthComponent::HealthComponent(void) {
	m_currentHP		= 0;
	m_maxHP			= 0;
}

/// <summary>
/// Initializes a new instance of the <see cref="HealthComponent"/> class.
/// </summary>
/// <param name="currentHP">The current hp.</param>
/// <param name="maxHP">The maximum hp.</param>
HealthComponent::HealthComponent(int currentHP, int maxHP) {
	m_currentHP		= currentHP;
	m_maxHP			= maxHP;
}

/// <summary>
/// Finalizes an instance of the <see cref="HealthComponent"/> class.
/// </summary>
HealthComponent::~HealthComponent(void) {
}
