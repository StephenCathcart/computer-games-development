#ifndef MENUITEMCOMPONENT_H
#define MENUITEMCOMPONENT_H

#include "GameComponent.h"

class MenuItemComponent : public GameComponent {
public:
	MenuItemComponent(void);
	MenuItemComponent(bool isActive);
	~MenuItemComponent(void);

	int		getIsActive() const { return m_isActive; }
	void	setIsActive(int isActive) { m_isActive = isActive; }

	void	update(float msec) {};

private:
	int		m_isActive;
};

#endif
