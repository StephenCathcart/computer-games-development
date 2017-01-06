#include "PositionComponent.h"

/// <summary>
/// Initializes a new instance of the <see cref="PositionComponent"/> class.
/// </summary>
PositionComponent::PositionComponent(void) {
	m_position = glm::vec3(0.0f, 0.0f, 0.0f);
	m_rotation = glm::vec3(0.0f, 0.0f, 0.0f);
}

/// <summary>
/// Initializes a new instance of the <see cref="PositionComponent"/> class.
/// </summary>
/// <param name="position">The position.</param>
/// <param name="rotation">The rotation.</param>
PositionComponent::PositionComponent(glm::vec3 position, glm::vec3 rotation) {
	m_position = position;
	m_rotation = rotation;
}

/// <summary>
/// Finalizes an instance of the <see cref="PositionComponent"/> class.
/// </summary>
PositionComponent::~PositionComponent(void) {
}
