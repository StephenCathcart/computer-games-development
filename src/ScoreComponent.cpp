#include "ScoreComponent.h"

/// <summary>
/// Initializes a new instance of the <see cref="ScoreComponent"/> class.
/// </summary>
ScoreComponent::ScoreComponent(void) {
	m_value = 0;
}

/// <summary>
/// Initializes a new instance of the <see cref="ScoreComponent"/> class.
/// </summary>
/// <param name="value">The value.</param>
ScoreComponent::ScoreComponent(int value) {
	m_value = value;
}

/// <summary>
/// Finalizes an instance of the <see cref="ScoreComponent"/> class.
/// </summary>
ScoreComponent::~ScoreComponent(void) {
}
