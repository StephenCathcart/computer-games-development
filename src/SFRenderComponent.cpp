#include "SFRenderComponent.h"

/// <summary>
/// Initializes a new instance of the <see cref="SFRenderComponent"/> class.
/// </summary>
/// <param name="m">The m.</param>
/// <param name="s">The s.</param>
SFRenderComponent::SFRenderComponent(sf::Texture& texture) 
	: m_texture(texture) {
}

/// <summary>
/// Finalizes an instance of the <see cref="SFRenderComponent"/> class.
/// </summary>
SFRenderComponent::~SFRenderComponent(void) {
}