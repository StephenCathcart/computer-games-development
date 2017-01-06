#ifndef PLAYERCOMPONENT_H
#define PLAYERCOMPONENT_H

#include "GameComponent.h"

/// <summary>
/// Player components save information on whether or not the player
/// is inclining or declining.
/// </summary>
class PlayerComponent : public GameComponent {
public:
	PlayerComponent();
	~PlayerComponent(void);

	void	update ( float msec ) {};

	bool	getIsInclining() const { return m_isInclining; }
	void	setIsInclining( bool isInclining ) { m_isInclining = isInclining; }
	bool	getIsDeclining() const { return m_isDeclining; }
	void	setIsDeclining( bool isDeclining ) { m_isDeclining = isDeclining; }

private:
	bool	m_isInclining;
	bool	m_isDeclining;
};

#endif