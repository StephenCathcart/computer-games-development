#ifndef ENEMYCOMPONENT_H
#define ENEMYCOMPONENT_H

#include "GameComponent.h"

enum EnemyType {
	STRAIGHT, CURVED,
};

/// <summary>
/// Enemy Component holds a damage value that is can do to player.
/// </summary>
class EnemyComponent : public GameComponent {
public:
	EnemyComponent(EnemyType m_enemyType, float damage, bool isActive);
	~EnemyComponent(void);

	void		update ( float msec ) {};

	float		getDamage() const { return m_damage; }
	EnemyType	getEnemyType() const { return m_enemyType; }
	void		setEnemyType(EnemyType enemyType) { m_enemyType = enemyType; }
	bool		getIsActive() { return m_isActive; }
	void		setIsActive(bool isActive) { m_isActive = isActive; }

private:
	float		m_damage;
	EnemyType	m_enemyType;
	bool		m_isActive;
};

#endif
