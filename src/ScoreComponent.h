#ifndef SCORECOMPONENT_H
#define SCORECOMPONENT_H

#include "GameComponent.h"

/// <summary>
/// Score components store a float value only.
/// </summary>
class ScoreComponent : public GameComponent {
public:
	ScoreComponent(void);
	ScoreComponent(int value);
	~ScoreComponent(void);

	int getValue() const { return m_value; }
	int setValue(int value) { m_value = value; }

	void update(float msec) {};

private:
	int  m_value;
};

#endif
