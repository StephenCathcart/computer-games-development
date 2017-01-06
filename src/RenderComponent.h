#ifndef RENDERCOMPONENT_H
#define RENDERCOMPONENT_H

#include "GameComponent.h"
#include "Mesh.h"
#include "Shader.h"

/// <summary>
/// Render components hold mesh and shader information.
/// </summary>
class RenderComponent : public GameComponent {
public:
	RenderComponent(Mesh& m, Shader& s);
	~RenderComponent(void);

	void	update ( float msec ) {};

	Mesh&	getMesh() const { return m_mesh; }
	Shader&	getShader() const { return m_shader; }

private:
	Mesh&	m_mesh;
	Shader& m_shader;
};

#endif