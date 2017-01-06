#ifndef PICKUPCOMPONENT_H
#define PICKUPCOMPONENT_H

#include "GameComponent.h"

enum PickUpType {
	POINTS = 0, BOMB, WEAPON, SHIELD,
};

/// <summary>
/// PickUpComponent allow the player to interact with the Entity i.e powerup/shield.
/// </summary>
class PickUpComponent : public GameComponent {
public:
	PickUpComponent(bool isActive, PickUpType m_pickUpType);
	~PickUpComponent(void);

	void update ( float msec ) {};

	PickUpType	getPickUpType() { return m_pickUpType; }
	bool getIsActive() { return m_isActive; }
	void setIsActive(bool isActive) { m_isActive = isActive; }

private:
	PickUpType  m_pickUpType;
	bool		m_isActive;
};

#endif