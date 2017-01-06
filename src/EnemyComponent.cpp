#include "EnemyComponent.h"

/// <summary>
/// Initializes a new instance of the <see cref="EnemyComponent"/> class.
/// </summary>
/// <param name="damage">The damage.</param>
EnemyComponent::EnemyComponent(EnemyType enemyType, float damage, bool isActive) {
	m_damage = damage;
	m_enemyType = enemyType;
	m_isActive = isActive;
}


/// <summary>
/// Finalizes an instance of the <see cref="EnemyComponent"/> class.
/// </summary>
EnemyComponent::~EnemyComponent(void) {
}
