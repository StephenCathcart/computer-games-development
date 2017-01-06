#ifndef POSITIONCOMPONENT_H
#define POSITIONCOMPONENT_H

#include "GameComponent.h"
#include "glm\glm.hpp"

/// <summary>
/// Position components hold vectors for XYZ position and 
/// XYZ rotation.
/// </summary>
class PositionComponent : public GameComponent {
public:
	PositionComponent(void);
	PositionComponent(glm::vec3 position, glm::vec3 rotation = glm::vec3(0.0f));
	~PositionComponent(void);

	void		update ( float msec ) {};

	void		setPosX(const float x) { m_position.x = x; };
	float		getPosX() const { return m_position.x; }
	void		setPosY(const float y) { m_position.y = y; };
	float		getPosY() const { return m_position.y; }
	void		setPosZ(const float z) { m_position.z = z; };
	float		getPosZ() const { return m_position.z; }

	void		setRotX(const float x) { m_rotation.x = x; };
	float		getRotX() const { return m_rotation.x; }
	void		setRotY(const float y) { m_rotation.y = y; };
	float		getRotY() const { return m_rotation.y; }
	void		setRotZ(const float z) { m_rotation.z = z; };
	float		getRotZ() const { return m_rotation.z; }

private:
	glm::vec3	m_position;
	glm::vec3	m_rotation;
};

#endif