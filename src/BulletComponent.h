#ifndef BULLETCOMPONENT_H
#define BULLETCOMPONENT_H

#include "GameComponent.h"

enum BulletType {
	LINEAR, SINE,
};

class BulletComponent  : public GameComponent {
public:
	BulletComponent(bool isActive);
	~BulletComponent(void);

	void update ( float msec ) {};

	bool getIsActive() { return m_isActive; }
	void setIsActive(bool isActive) { m_isActive = isActive; }

	int getDamage() { return m_damage; }
	BulletType getBulletType() { return m_bulletType; }
	void setBulletType(BulletType bulletType) { m_bulletType = bulletType; }

private:
	bool		m_isActive;
	BulletType	m_bulletType;
	int			m_damage;
};

#endif