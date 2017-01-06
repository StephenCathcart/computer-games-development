#ifndef CRATECOMPONENT_H
#define CRATECOMPONENT_H

#include "GameComponent.h"

class CrateComponent : public GameComponent {
public:
	CrateComponent(void);
	~CrateComponent(void);

	void update ( float msec ) {};
};

#endif