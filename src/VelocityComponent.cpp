#include "VelocityComponent.h"

/// <summary>
/// Initializes a new instance of the <see cref="VelocityComponent"/> class.
/// </summary>
VelocityComponent::VelocityComponent(void) {

/// <summary>
/// Initializes a new instance of the <see cref="VelocityComponent"/> class.
/// </summary>
/// <param name="velocity">The velocity.</param>
	m_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
}

VelocityComponent::VelocityComponent(glm::vec3 velocity) {
	m_velocity = velocity;
}

/// <summary>
/// Finalizes an instance of the <see cref="VelocityComponent"/> class.
/// </summary>
VelocityComponent::~VelocityComponent(void) {
}
