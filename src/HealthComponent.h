#ifndef HEALTHCOMPONENT_H
#define HEALTHCOMPONENT_H

#include "GameComponent.h"

/// <summary>
/// Health components hold float values for current and max health.
/// </summary>
class HealthComponent : public GameComponent {
public:
	HealthComponent(void);
	HealthComponent(int currentHP, int maxHP);
	~HealthComponent(void);

	void	update ( float msec ) {};

	void	setCurrentHP(const int amount) { 
				(amount < m_maxHP) ? m_currentHP = amount : m_currentHP = m_maxHP; 
	}
	int		getCurrentHP() const { return m_currentHP; }
	void	setMaxHP(const float amount) { m_maxHP = amount; };
	int		getMaxHP() const { return m_maxHP; }
	bool	getIsAlive() const { return m_currentHP > 0; }

private:
	int	m_currentHP;
	int	m_maxHP;
};

#endif