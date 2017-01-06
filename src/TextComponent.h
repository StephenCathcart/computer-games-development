#ifndef TEXTCOMPONENT_H
#define TEXTCOMPONENT_H

#include "GameComponent.h"
#include <SFML\Graphics\Text.hpp>

using std::string;

/// <summary>
/// Text components hold sfml font and text data.
/// </summary>
class TextComponent : public GameComponent {
public:
	TextComponent(string fontPath, string text);
	~TextComponent(void);

	void		update ( float msec ) {};

	sf::Text&	getText() { return m_text; }

private:
	sf::Font	m_font;
	sf::Text	m_text;
};

#endif
