#ifndef SHIELDCOMPONENT_H
#define SHIELDCOMPONENT_H

#include "GameComponent.h"

class ShieldComponent : public GameComponent {
public:
	ShieldComponent(void);
	~ShieldComponent(void);

	void update ( float msec ) {};
};

#endif
