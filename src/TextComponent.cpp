#include "TextComponent.h"

/// <summary>
/// Initializes a new instance of the <see cref="TextComponent"/> class.
/// </summary>
/// <param name="fontPath">The font path.</param>
/// <param name="text">The text.</param>
TextComponent::TextComponent(string fontPath, string text) {
	if (!m_font.loadFromFile(fontPath)) {
		throw "Error loading font!"; 
	}
	m_text.setFont(m_font);
	m_text.setString(text);
}

/// <summary>
/// Finalizes an instance of the <see cref="TextComponent"/> class.
/// </summary>
TextComponent::~TextComponent(void){
}
