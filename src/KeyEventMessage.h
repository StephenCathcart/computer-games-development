#ifndef KEYEVENTMESSAGE_H
#define KEYEVENTMESSAGE_H

#include "BaseMessage.h"
#include <SFML\Window\Event.hpp>

/// <summary>
/// Stores keyboard event information. Its message type is
/// of KEYEVENT. 
/// </summary>
class KeyEventMessage : public BaseMessage {
public:
	KeyEventMessage(sf::Event e);
	~KeyEventMessage(void);

	MessageType getMessageType() const { return m_type; }
	sf::Event	getPayload() const { return m_event; }

private:
	MessageType m_type;
	sf::Event	m_event;
};

#endif