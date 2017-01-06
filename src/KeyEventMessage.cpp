#include "KeyEventMessage.h"

/// <summary>
/// Initializes a new instance of the <see cref="KeyEventMessage"/> class.
/// </summary>
/// <param name="e">The event.</param>
KeyEventMessage::KeyEventMessage(sf::Event e) {
	m_type	= MessageType::KEYEVENT;
	m_event = e;
}

/// <summary>
/// Finalizes an instance of the <see cref="KeyEventMessage"/> class.
/// </summary>
KeyEventMessage::~KeyEventMessage(void) {
}
