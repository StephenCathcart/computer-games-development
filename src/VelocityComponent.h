#ifndef VELOCITYCOMPONENT_H
#define VELOCITYCOMPONENT_H

#include "GameComponent.h"
#include "glm\glm.hpp"

/// <summary>
/// Velocity components holds a vector for XYZ velocities.
/// </summary>
class VelocityComponent : public GameComponent {
public:
	VelocityComponent(void);
	VelocityComponent(glm::vec3 velocity);
	~VelocityComponent(void);

	void		update ( float msec ) {};

	void		setVX(const float vX) { m_velocity.x = vX; };
	float		getVX() const { return m_velocity.x; }
	void		setVY(const float vY) { m_velocity.y = vY; };
	float		getVY() const { return m_velocity.y; }
	void		setVZ(const float vZ) { m_velocity.z = vZ; };
	float		getVZ() const { return m_velocity.z; }

private:
	glm::vec3	m_velocity;
};

#endif