#ifndef MULTIPLIERCOMPONENT_H
#define MULTIPLIERCOMPONENT_H

#include "GameComponent.h"

/// <summary>
/// Multiplier components hold a float value only.
/// </summary>
class MultiplierComponent : public GameComponent {
public:
	MultiplierComponent(void);
	~MultiplierComponent(void);

	void  update ( float msec ) {};

	float getValue() const { return m_value; }
	float setValue(float value) { m_value = value; }

private:
	float m_value;
};

#endif
