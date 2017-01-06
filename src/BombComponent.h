#ifndef BOMBCOMPONENT_H
#define BOMBCOMPONENT_H

#include "GameComponent.h"

class BombComponent : public GameComponent {
public:
	BombComponent(void);
	BombComponent(int amount);
	~BombComponent(void);

	void update ( float msec ) {};

	int getAmount() const { return m_amount; }

	void increment() {
		m_amount++;
	}

	void decrement() {
		m_amount = (m_amount == 0) ? 0 : --m_amount; 
	}

private:
	int	m_amount;
};

#endif
