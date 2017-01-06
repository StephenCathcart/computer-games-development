#ifndef SF_RENDERCOMPONENT_H
#define SF_RENDERCOMPONENT_H

#include "GameComponent.h"
#include <SFML/Graphics/Texture.hpp>

class SFRenderComponent : public GameComponent {
public:
	SFRenderComponent(sf::Texture& texture);
	~SFRenderComponent(void);

	void			update ( float msec ) {};
	sf::Texture&	getTexture() const { return m_texture; }

private:
	sf::Texture&	m_texture;
};

#endif
