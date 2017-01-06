#include "RenderComponent.h"

/// <summary>
/// Initializes a new instance of the <see cref="RenderComponent"/> class.
/// </summary>
/// <param name="m">The m.</param>
/// <param name="s">The s.</param>
RenderComponent::RenderComponent(Mesh& m, Shader& s) 
	: m_mesh(m), m_shader(s) {
}

/// <summary>
/// Finalizes an instance of the <see cref="RenderComponent"/> class.
/// </summary>
RenderComponent::~RenderComponent(void) {
}
